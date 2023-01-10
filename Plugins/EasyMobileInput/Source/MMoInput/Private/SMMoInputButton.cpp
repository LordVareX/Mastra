// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "SMMoInputButton.h"
#include "MMoInputButton.h"


// Be called by UMMoInputButton::RebuildWidget
void SMMoInputButton::Construct(const FArguments& InArgs) {

	// Just set to default values
	this->bTouched = false;
	this->bNotifyTouchDown = false;
	this->bNotifyTouchUp = false;
	this->CapturedPointerIndex = -1;
	this->bHandleEvent = false;
	this->PrevScaleFactor = 0.f;
	this->bFirstMove = false;

	this->Default_CorrectedCenter = FVector2D::ZeroVector;
	this->Default_CorrectedVisualSize = FVector2D::ZeroVector;
	this->Icon_CorrectedVisualSize = FVector2D::ZeroVector;

	this->Range_VisualCenter = FVector2D::ZeroVector;
	this->Range_CorrectedVisualSize = FVector2D::ZeroVector;

	this->Top_Location = FVector2D::ZeroVector;
	this->Top_CorrectedVisualSize = FVector2D::ZeroVector;

	// Obtaining values from UMMoInputButton
	this->OnTouchDown = InArgs._OnTouchDown;
	this->OnTouchUp = InArgs._OnTouchUp;
	this->OnTouchMove = InArgs._OnTouchMove;
}

int32 SMMoInputButton::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
			FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {

	int32 RetLayerId = LayerId;
	
	const FSlateBrush* NormalBrush = nullptr;
	const FSlateBrush* DefaultBrush = this->Default.Get();
	const FSlateBrush* PressedBrush = this->Pressed.Get();
	
	// Draw the appearance of the button when it is pressed or released
	if (DefaultBrush && DefaultBrush->DrawAs != ESlateBrushDrawType::NoDrawType) {
		NormalBrush = DefaultBrush;
	}

	if (this->bTouched && PressedBrush != nullptr && PressedBrush->DrawAs != ESlateBrushDrawType::NoDrawType) {
		NormalBrush = PressedBrush;
	}

	if (NormalBrush) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			RetLayerId++,
			AllottedGeometry.ToPaintGeometry(
				this->Default_CorrectedCenter - (0.5f * this->Default_CorrectedVisualSize),
				this->Default_CorrectedVisualSize
			),
			NormalBrush,
			ESlateDrawEffect::None,
			NormalBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
		);
	}

	// Draw the icon
	const FSlateBrush* IconBrush = this->Icon.Get();
	if (IconBrush && IconBrush->DrawAs != ESlateBrushDrawType::NoDrawType) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			RetLayerId++,
			AllottedGeometry.ToPaintGeometry(
				this->Default_CorrectedCenter - (0.5f * this->Icon_CorrectedVisualSize),
				this->Icon_CorrectedVisualSize
			),
			IconBrush,
			ESlateDrawEffect::None,
			IconBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
		);
	}


	// Draw the background image of the joystick
	if (this->Owner->bWithJoystick && this->bTouched) {
		const FSlateBrush* RangeBrush = this->Range.Get();
		if (RangeBrush && RangeBrush->DrawAs != ESlateBrushDrawType::NoDrawType) {
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				RetLayerId++,
				AllottedGeometry.ToPaintGeometry(
					this->Range_VisualCenter - 0.5f * this->Range_CorrectedVisualSize,
					this->Range_CorrectedVisualSize
				),
				RangeBrush,
				ESlateDrawEffect::None,
				RangeBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
			);
		}

		// Draw the thumb image of the joystick
		const FSlateBrush* TopBrush = this->Top.Get();
		if (this->Owner->bWithJoystick && TopBrush && TopBrush->DrawAs != ESlateBrushDrawType::NoDrawType) {
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				RetLayerId++,
				AllottedGeometry.ToPaintGeometry(
					this->Range_VisualCenter + this->Top_Location - this->Top_CorrectedVisualSize * 0.5f,
					this->Top_CorrectedVisualSize
				),
				TopBrush,
				ESlateDrawEffect::None,
				TopBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
			);
		}
	}

	return RetLayerId;
}


FVector2D SMMoInputButton::ComputeDesiredSize(float) const {
	return FVector2D(100.f, 100.f);
}


FReply SMMoInputButton::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	
	// Already start just ignore
	if (this->CapturedPointerIndex != -1) {
		return FReply::Unhandled();
	}

	// Transforms AbsoluteCoordinate into the local space of this Geometry
	FVector2D TmpLocalCoord = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());


	// Only handle touches within the range of the button
	if (PositionIsInside(this->Default_CorrectedCenter, TmpLocalCoord, this->Default_CorrectedVisualSize, this->Owner->bCircular)) {
		this->bTouched = true;
		this->CurrentLocalCoord = TmpLocalCoord;
		this->PrevLocalCoord = TmpLocalCoord;
		this->CapturedPointerIndex = InTouchEvent.GetPointerIndex();
		this->bNotifyTouchDown = false;
		this->bNotifyTouchUp = false;
		this->bHandleEvent = false;

		// Set the center of the joystick
		this->Range_VisualCenter = TmpLocalCoord;

		// Notify Blueprint
		this->HandleTouch(this->CurrentLocalCoord);

		// Track
		return FReply::Handled().CaptureMouse(SharedThis(this));
	}

	return FReply::Unhandled();
}


FReply SMMoInputButton::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	if (this->CapturedPointerIndex != InTouchEvent.GetPointerIndex()) {
		return FReply::Unhandled();
	}

	// bugfix: ue427
	if (!this->bFirstMove) {
		this->bFirstMove = true;
		return FReply::Handled();
	}
	

	FVector2D TmpLocalCoord = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());
	this->PrevLocalCoord = this->CurrentLocalCoord;
	this->CurrentLocalCoord = TmpLocalCoord;
	this->bHandleEvent = true;

	// Notify Blueprint
	this->HandleTouch(this->CurrentLocalCoord);
	return FReply::Handled();
}


FReply SMMoInputButton::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	if (this->CapturedPointerIndex != InTouchEvent.GetPointerIndex()) {
		return FReply::Unhandled();
	}

	this->PrevLocalCoord = this->CurrentLocalCoord;
	this->bTouched = false;
	this->CapturedPointerIndex = -1;
	this->bHandleEvent = false;
	//this->Top_Location = FVector2D::ZeroVector;

	return FReply::Handled().ReleaseMouseCapture();
}

void SMMoInputButton::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) {
	float ScaleFactor = GetScaleFactor(AllottedGeometry);
	if (this->PrevScaleFactor != ScaleFactor) {
		
		// Recalculate center and size
		this->Default_CorrectedCenter = FVector2D(Owner->Center.X * AllottedGeometry.GetLocalSize().X, Owner->Center.Y * AllottedGeometry.GetLocalSize().Y);
		this->Default_CorrectedVisualSize = this->Owner->ButtonSize;
		this->Icon_CorrectedVisualSize =  this->Owner->IconSize;
		this->Range_CorrectedVisualSize =  this->Owner->RangeSize;
		this->Top_CorrectedVisualSize =  this->Owner->TopSize;
	}
	this->PrevScaleFactor = ScaleFactor;

	this->HandlePress();
	this->HandleInput();
}

void SMMoInputButton::HandleTouch(const FVector2D& LocalCoord) {
	// The vector of the touch point to the center of the background image of the joystick
	FVector2D Offset = LocalCoord - this->Range_VisualCenter;

	if (Offset == FVector2D::ZeroVector) {
		// Not moving
		this->Top_Location = FVector2D::ZeroVector;
	} else {
		if (this->Range_CorrectedVisualSize.X <= 0 || this->Range_CorrectedVisualSize.Y <= 0) {
			return;
		}

		float DistanceToTouchSqr =  Offset.SizeSquared();

		// tan()  atan2() https://zh.m.wikipedia.org/zh-hans/Atan2
		float Angle = FMath::Atan2(Offset.Y, Offset.X);
		float CosAngle = FMath::Cos(Angle);
		float SinAngle = FMath::Sin(Angle);

		// Calculate the shortest distance from touch point to the ellipse (the background of the joystick)
		float XTerm = CosAngle /( this->Range_CorrectedVisualSize.X * 0.5f);
		float YTerm = SinAngle /(this->Range_CorrectedVisualSize.Y * 0.5f);
		float DistanceToEdge = FMath::InvSqrt(XTerm * XTerm + YTerm * YTerm);

		// Make sure the position of the thumb image is inside the ellipse
		if (DistanceToTouchSqr > FMath::Square(DistanceToEdge)) {
			this->Top_Location = FVector2D(DistanceToEdge * CosAngle, DistanceToEdge * SinAngle);
		} else {
			// Inside the ellipse
			this->Top_Location = Offset;
		}
	}
}

void SMMoInputButton::HandlePress() {
	if (this->bTouched) {
		if (this->bNotifyTouchDown) {
			return;
		}
		this->bNotifyTouchDown = true;

		FVector2D Offset = this->Top_Location;

		this->OnTouchDown.ExecuteIfBound(Offset);
	} else {
		if (!this->bNotifyTouchDown || this->bNotifyTouchUp) {
			return;
		}
		this->bNotifyTouchUp = true;

		//FVector2D Offset = this->Top_Location - this->Range_VisualCenter;
		FVector2D Offset = this->Top_Location;
		this->OnTouchUp.ExecuteIfBound(Offset);
	}
}

void SMMoInputButton::HandleInput() {
	if (this->bHandleEvent) {
		FVector2D Offset = this->Top_Location;
		this->OnTouchMove.ExecuteIfBound(Offset);
	}
}


void SMMoInputButton::SetIcon(const TAttribute<const FSlateBrush*>& InIcon) {
	this->Icon = InIcon;
}

void SMMoInputButton::SetDefault(const TAttribute<const FSlateBrush*>& InDefault) {
	this->Default = InDefault;
}

void SMMoInputButton::SetPressed(const TAttribute<const FSlateBrush*>& InPressed) {
	this->Pressed = InPressed;
}

void SMMoInputButton::SetRange(const TAttribute<const FSlateBrush*>& InRange) {
	this->Range = InRange;
}

void SMMoInputButton::SetTop(const TAttribute<const FSlateBrush*>& InTop) {
	this->Top = InTop;
}
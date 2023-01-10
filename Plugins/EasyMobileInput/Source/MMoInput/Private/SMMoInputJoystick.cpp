// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "SMMoInputJoystick.h"
#include "MMoInputJoystick.h"
#include "Events.h"
#include "Components/Button.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"


void SMMoInputJoystick::Construct(const FArguments& InArgs) {
	// before call SetOwner
	this->CapturedPointerIndex = -1;
	this->PrevScaleFactor = 0.f;
	this->bHandleEvent = false;

	this->Range_CorrectedCenter = FVector2D::ZeroVector;
	this->Range_VisualCenter = FVector2D::ZeroVector;
	this->Range_CorrectedVisualSize = FVector2D::ZeroVector;

	this->Top_Location = FVector2D::ZeroVector;
	this->Top_CorrectedVisualSize = FVector2D::ZeroVector;

	this->CurrentState = EJoyStickState::Inactivated;
	this->bTargetFadeColor = InArgs._FadeOutColor;
	this->bNeedFade = InArgs._bNeedFadeOut;

	/* show nothing at first
	if (this->bNeedFade) {
		this->CurrentColor = this->bTargetFadeColor;
	} else {
		this->CurrentColor = FLinearColor::White;
	}
	*/
	
	// show joystick at first
	this->CurrentColor = FLinearColor::White;
	
	this->bFading = false;
	this->FadingDeltaTime = 0.f;
	this->bResetting = false;
	this->ResettingDeltaTime = 0.f;
	this->RepositioningSize = InArgs._RepositioningSize;
	this->bRepositioningWithinSlot = InArgs._bRepositioningWithinSlot;
	this->bNeedReset = InArgs._bNeedReset;
	//this->RepositioningPosition = InArgs._RepositioningPosition;
}

int32 SMMoInputJoystick::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
								FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {
	int32 RetLayerId = LayerId;

	//if (this->Owner->bWithJoystick && this->bTouched) {
	const FSlateBrush* RangeBrush = this->Range.Get();
	
	// The background image of the joystick
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
			RangeBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * GetCurrentColor()
		);
	}

	// The thumb image of the joystick
	const FSlateBrush* TopBrush = this->Top.Get();
	if (TopBrush && TopBrush->DrawAs != ESlateBrushDrawType::NoDrawType) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			RetLayerId++,
			AllottedGeometry.ToPaintGeometry(
				this->Range_VisualCenter + this->Top_Location - this->Top_CorrectedVisualSize * 0.5f,
				this->Top_CorrectedVisualSize
			),
			TopBrush,
			ESlateDrawEffect::None,
			TopBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * GetCurrentColor()
		);
	}
	//}

	return RetLayerId;
}

FVector2D SMMoInputJoystick::ComputeDesiredSize(float) const {
	return FVector2D(100.f, 100.f);
}

FReply SMMoInputJoystick::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	if (this->CapturedPointerIndex != -1 || this->Owner == nullptr) {
		return FReply::Unhandled();
	}

	FVector2D TmpLocalCoord = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());


	
	if (this->bRepositioningWithinSlot) {
		AlignBoxIntoScreen(TmpLocalCoord, this->Range_CorrectedVisualSize, MyGeometry.GetLocalSize());
	}

	// Need Repositioning

#if (ENGINE_MAJOR_VERSION == 5)
	if (this->Owner->bNeedRepositioning && PositionIsInside(this->Range_CorrectedCenter,  TmpLocalCoord, this->Owner->GetRepositioningSize(), false)) {
		this->Range_VisualCenter = TmpLocalCoord;
	}
#else	
	if (this->Owner->bNeedRepositioning && PositionIsInside(this->Range_CorrectedCenter,  TmpLocalCoord, this->RepositioningSize, false)) {
		this->Range_VisualCenter = TmpLocalCoord;
	}
#endif
	
	if (PositionIsInside(this->Range_VisualCenter, TmpLocalCoord, this->Range_CorrectedVisualSize, true)) {
		this->CurrentState = EJoyStickState::Activated;
		this->CurrentColor = FLinearColor::White;
		this->bFading = false;
		this->bResetting = false;
		
		this->CapturedPointerIndex = InTouchEvent.GetPointerIndex();
		this->HandleTouch(TmpLocalCoord);
		

		return FReply::Handled().CaptureMouse(SharedThis(this));
	}

	return FReply::Unhandled();
}


FReply SMMoInputJoystick::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	if (this->CapturedPointerIndex != InTouchEvent.GetPointerIndex()) {
		return FReply::Unhandled();
	}

	FVector2D TmpLocalCoord = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());
	this->CurrentState = EJoyStickState::Activated;
	//this->bHandleEvent = true;

	this->HandleTouch(TmpLocalCoord);
	return FReply::Handled();
}


FReply SMMoInputJoystick::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	if (this->CapturedPointerIndex != InTouchEvent.GetPointerIndex()) {
		return FReply::Unhandled();
	}

	this->CapturedPointerIndex = -1;
	this->bHandleEvent = true;
	this->Top_Location = FVector2D::ZeroVector;
	this->CurrentState = EJoyStickState::Inactivated;
	this->FadingDeltaTime = 0.f;
	this->ResettingDeltaTime = 0.f;
	

	return FReply::Handled().ReleaseMouseCapture();
}

void SMMoInputJoystick::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) {
	if (this->Owner == nullptr){
		return;
	}
	
	// deal fading color
	if (this->CurrentState == EJoyStickState::Inactivated && this->bNeedFade) {
		this->FadingDeltaTime += InDeltaTime;
		if (this->FadingDeltaTime >= this->Owner->FadeOutDelay) {
			this->bFading = true;
			this->FadingDeltaTime = 0.f;
		}
	}

	if (this->bFading) {
		float FadeOutSpeed = this->Owner->FadeOutSpeed;
		if (FadeOutSpeed < 0.f) {
			FadeOutSpeed = 4.f;
		}
		this->CurrentColor = FLinearColor::LerpUsingHSV(this->CurrentColor, this->Owner->FadeOutColor, InDeltaTime * FadeOutSpeed);
	}

	// deal reset joystick
	if (this->CurrentState == EJoyStickState::Inactivated && this->Owner->bNeedRepositioning) {
		this->ResettingDeltaTime += InDeltaTime;
		if (this->bNeedReset && this->ResettingDeltaTime >= this->Owner->ResetPositionDelay) {
			this->bResetting = true;
			this->ResettingDeltaTime = 0.f;
		}
	}
	
	if (this->bResetting) {
		this->Range_VisualCenter = this->Range_CorrectedCenter;
	}
	
	
	float ScaleFactor = GetScaleFactor(AllottedGeometry);
	if (this->PrevScaleFactor != ScaleFactor) {

		this->Range_CorrectedCenter = FVector2D(0.5f * AllottedGeometry.GetLocalSize().X, 0.5f * AllottedGeometry.GetLocalSize().Y);
		this->Range_VisualCenter = this->Range_CorrectedCenter;


		this->Range_CorrectedVisualSize = this->Owner->RangeSize;
		this->Top_CorrectedVisualSize = this->Owner->TopSize;
	}

	this->PrevScaleFactor = ScaleFactor;
	if (this->CapturedPointerIndex == -1 && this->bHandleEvent == false) {
		return;
	}

	if (this->Range_CorrectedVisualSize.X <= 0 || this->Range_CorrectedVisualSize.Y <= 0) {
		return;
	}

	this->bHandleEvent = false;
		
	// Get the corrected thumb offset scale (now allows ellipse instead of assuming square)
	FVector2D ThumbScaledOffset = FVector2D(this->Top_Location.X * 2.0f / this->Range_CorrectedVisualSize.X, this->Top_Location.Y * 2.0f / Range_CorrectedVisualSize.Y);
	float ThumbSquareSum = ThumbScaledOffset.X * ThumbScaledOffset.X + ThumbScaledOffset.Y * ThumbScaledOffset.Y;
	float ThumbMagnitude = FMath::Sqrt(ThumbSquareSum);
	FVector2D ThumbNormalized = FVector2D(0.f, 0.f);
	if (ThumbSquareSum > SMALL_NUMBER) {
		const float Scale = 1.0f / ThumbMagnitude;
		ThumbNormalized = FVector2D(ThumbScaledOffset.X * Scale, ThumbScaledOffset.Y * Scale);
	}

	// Find the scale to apply to ThumbNormalized vector to project onto unit square
	float ToSquareScale = fabs(ThumbNormalized.Y) > fabs(ThumbNormalized.X) ? FMath::Sqrt((ThumbNormalized.X * ThumbNormalized.X) / (ThumbNormalized.Y * ThumbNormalized.Y) + 1.0f)
		: ThumbNormalized.X == 0.0f ? 1.0f : FMath::Sqrt((ThumbNormalized.Y * ThumbNormalized.Y) / (ThumbNormalized.X * ThumbNormalized.X) + 1.0f);

	// Apply proportional offset corrected for projection to unit square
	FVector2D NormalizedOffset = ThumbNormalized * 1.f * ThumbMagnitude * ToSquareScale;

	// now pass the fake joystick events to the game
	const FGamepadKeyNames::Type XAxis = (this->Owner->MainInputKey.IsValid() ? this->Owner->MainInputKey.GetFName() : FGamepadKeyNames::LeftAnalogX);
	const FGamepadKeyNames::Type YAxis = (this->Owner->AltInputKey.IsValid() ? this->Owner->AltInputKey.GetFName() : FGamepadKeyNames::RightAnalogY);

	FSlateApplication::Get().SetAllUserFocusToGameViewport();
	FSlateApplication::Get().OnControllerAnalog(XAxis, 0, NormalizedOffset.X);
	FSlateApplication::Get().OnControllerAnalog(YAxis, 0, -NormalizedOffset.Y);
}

void SMMoInputJoystick::HandleTouch(const FVector2D& LocalCoord) {
	// The vector of the touch point to the center of the background image of the joystick
	FVector2D Offset = LocalCoord - this->Range_VisualCenter;
	if (Offset == FVector2D::ZeroVector) {
		// Not moving
		this->Top_Location = FVector2D::ZeroVector;
	} else {
		if (this->Range_CorrectedVisualSize.X <= 0 || this->Range_CorrectedVisualSize.Y <= 0) {
			return;
		}
		// tan()  atan2() https://zh.m.wikipedia.org/zh-hans/Atan2
		float DistanceToTochSqr = Offset.SizeSquared(); 

		float Angle = FMath::Atan2(Offset.Y, Offset.X);
		float CosAngle = FMath::Cos(Angle);
		float SinAngle = FMath::Sin(Angle);

		// Calculate the shortest distance from touch point to the ellipse (the background of the joystick)
		float XTerm = CosAngle / (this->Range_CorrectedVisualSize.X * 0.5f);
		float YTerm = SinAngle / (this->Range_CorrectedVisualSize.Y * 0.5f);
		float DistanceToEdge = FMath::InvSqrt(XTerm * XTerm + YTerm * YTerm);

		// Make sure the position of the thumb image is inside the ellipse
		if (DistanceToTochSqr > FMath::Square(DistanceToEdge)) {
			this->Top_Location = FVector2D(DistanceToEdge * CosAngle, DistanceToEdge * SinAngle);
		} else {
			this->Top_Location = Offset;
		}
	}
}


void SMMoInputJoystick::SetRange(const TAttribute<const FSlateBrush*>& InRange) {
	this->Range = InRange;
}

void SMMoInputJoystick::SetTop(const TAttribute<const FSlateBrush*>& InTop) {
	this->Top = InTop;
}


FLinearColor SMMoInputJoystick::GetCurrentColor() const {
	return this->CurrentColor;
}
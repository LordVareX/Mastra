// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "SMMoInputTouch.h"
#include "MMoInputTouch.h"





FVector2D SMMoInputTouch::ComputeDesiredSize(float) const {
	return FVector2D(100.f, 100.f);
}

int32 SMMoInputTouch::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, 
							  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {

	int32 RetLayerId = LayerId;
	FLinearColor DrawColor = this->Owner->DeActiveColor;
	if (this->CurrentNumOfTouches > 0) {
		DrawColor = this->Owner->ActiveColor;
	}

	/*				
	* 0,0 ------------->
	*  |
	*  |
	*  |
	*  |
	*  |
	* \|/
	*/

	if (this->Owner->Image1.IsValid()) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			RetLayerId++,
			AllottedGeometry.ToPaintGeometry(
				this->CorrectedCenter - (this->CorrectedVisualSize * 0.5f) /*ƫ��*/,
				this->CorrectedVisualSize /*��С*/
			),
			this->Owner->Image1->GetSlateBrush(),
			ESlateDrawEffect::None,
			DrawColor
		);
	}

	return RetLayerId;
}

FReply SMMoInputTouch::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {
	if (this->CapturedPointerIndex != -1) {
		return FReply::Unhandled();
	}
		
	FVector2D TmpLocalCoord = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());
	//UE_LOG(LogTemp, Log, TEXT("Start Touch: X=%f Y=%f"), TmpLocalCoord.X, TmpLocalCoord.Y);
	//UE_LOG(LogTemp, Log, TEXT("Corrected Center: X=%f Y=%f"), this->CorrectedCenter.X, this->CorrectedCenter.Y);
	//UE_LOG(LogTemp, Log, TEXT("Corrected Visual Size: X=%f Y=%f"), this->CorrectedVisualSize.X, this->CorrectedVisualSize.Y);

	if (PositionIsInside(this->CorrectedCenter, TmpLocalCoord, this->CorrectedVisualSize, this->Owner->bCircular)) {
		this->bTouched = true;
		this->CurrentLocalCoord = TmpLocalCoord;
		this->PrevLocalCoord = TmpLocalCoord;
		this->CapturedPointerIndex = InTouchEvent.GetPointerIndex(); 
		this->bNotifyTouchDown = false;
		this->bNotifyTouchUp = false;
		this->bHandleEvent = false;

		return FReply::Handled().CaptureMouse(SharedThis(this));
	}

	this->bTouched = false;
	this->CapturedPointerIndex = -1;
	return FReply::Unhandled();
}

FReply SMMoInputTouch::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {

	if (this->CapturedPointerIndex != InTouchEvent.GetPointerIndex()) {
		return FReply::Unhandled();
	}

	FVector2D TmpLocalCoord = MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition());

	if (!PositionIsInside(this->CorrectedCenter, TmpLocalCoord, CorrectedVisualSize, this->Owner->bCircular)) {
			//UE_LOG(LogTemp, Log, TEXT("Leave Touch: X=%f Y=%f"), TmpLocalCoord.X, TmpLocalCoord.Y);
			//UE_LOG(LogTemp, Log, TEXT("Corrected Center: X=%f Y=%f"), this->CorrectedCenter.X, this->CorrectedCenter.Y);
			//UE_LOG(LogTemp, Log, TEXT("Corrected Visual Size: X=%f Y=%f"), this->CorrectedVisualSize.X, this->CorrectedVisualSize.Y);

			this->bTouched = false;
			this->CapturedPointerIndex = -1;
			this->bHandleEvent = false;
			return FReply::Handled().ReleaseMouseCapture();
	}

	this->PrevLocalCoord = this->CurrentLocalCoord;
	this->CurrentLocalCoord = TmpLocalCoord;
	this->bHandleEvent = true;

	return FReply::Handled();
}

FReply SMMoInputTouch::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) {

	if (this->CapturedPointerIndex != InTouchEvent.GetPointerIndex()) {
		return FReply::Unhandled();
	}

	UE_LOG(LogTemp, Log, TEXT("Ended Touch: X=%f Y=%f"), this->CurrentLocalCoord.X, this->CurrentLocalCoord.Y);
	UE_LOG(LogTemp, Log, TEXT("Corrected Center: X=%f Y=%f"), this->CorrectedCenter.X, this->CorrectedCenter.Y);
	UE_LOG(LogTemp, Log, TEXT("Corrected Visual Size: X=%f Y=%f"), CorrectedVisualSize.X, CorrectedVisualSize.Y);

	this->PrevLocalCoord = this->CurrentLocalCoord;
	this->bTouched = false;
	this->CapturedPointerIndex = -1;
	this->bHandleEvent = false;

	return FReply::Handled().ReleaseMouseCapture();
}

/*
FReply SMMoInputTouch::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return this->OnTouchStarted(MyGeometry, MouseEvent);
}
*/

void SMMoInputTouch::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) {
	//TODO
	
	if (this->bTouched) {
		TouchedPassed += InDeltaTime;
	}


	this->HandlePress();
	this->HandleInput();


	if (Owner->Center.X > 1) {
		Owner->Center.X = 1;
	}

	if (Owner->Center.Y > 1) {
		Owner->Center.Y = 1;
	}

	if (Owner->Center.X < 0) {
		Owner->Center.X = 0;
	}

	if (Owner->Center.Y < 0) {
		Owner->Center.Y = 0; 
	}

	float ScaleFactor = GetScaleFactor(AllottedGeometry);


	this->CorrectedCenter = FVector2D(Owner->Center.X * AllottedGeometry.GetLocalSize().X, Owner->Center.Y * AllottedGeometry.GetLocalSize().Y);

	this->CorrectedVisualSize = AllottedGeometry.GetLocalSize();

}

void SMMoInputTouch::HandlePress() {
	if (this->bTouched) {
		if (this->bNotifyTouchDown) {
			return;
		}
		this->bNotifyTouchDown = true;

		if (this->Owner->PressKey.IsValid()) {
			FSlateApplication::Get().OnControllerButtonPressed(this->Owner->PressKey.GetFName(), 0, false);
		}

		OnTouchDown.ExecuteIfBound(this->CurrentLocalCoord);

	} else {
		if (!this->bNotifyTouchDown || this->bNotifyTouchUp) {
			return;
		}
		this->bNotifyTouchUp = true;

		if (this->Owner->PressKey.IsValid()) {
			FSlateApplication::Get().OnControllerButtonReleased(this->Owner->PressKey.GetFName(), 0, false);
		}

		OnTouchUp.ExecuteIfBound(this->CurrentLocalCoord);
	}
}

void SMMoInputTouch::HandleInput() {
	if (this->bHandleEvent) {
		OnTouchMove.ExecuteIfBound(this->CurrentLocalCoord);
	}

	if (this->Owner->MainInputKey.IsValid()) {

	}

	if (this->Owner->AltInputKey.IsValid()) {

	}
}
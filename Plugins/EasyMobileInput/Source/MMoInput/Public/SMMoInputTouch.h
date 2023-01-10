// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "Runtime/SlateCore/Public/Widgets/SLeafWidget.h"
#include "Events.h"

class MMOINPUT_API SMMoInputTouch : public SLeafWidget {
public:
	SLATE_BEGIN_ARGS(SMMoInputTouch)
	{}
	SLATE_EVENT(FOnTouchDown, OnTouchDown)
	SLATE_EVENT(FOnTouchUp, OnTouchUp)
	SLATE_EVENT(FOnTouchMove, OnTouchMove)
	SLATE_END_ARGS()
	
	// SNew -> SWidgetConstruct -> Construct
	void Construct(const FArguments& InArgs) {
		this->TouchedPassed = 0;
		this->bTouched = false;
		this->CapturedPointerIndex = -1;
		this->bNotifyTouchDown = false;
		this->bNotifyTouchUp = false;
		this->bHandleEvent = false;
		//this->VisualCenter = FVector2D::ZeroVector;
		this->CorrectedCenter = FVector2D::ZeroVector;
		

		this->OnTouchDown = InArgs._OnTouchDown;
		this->OnTouchUp = InArgs._OnTouchUp;
		this->OnTouchMove = InArgs._OnTouchMove;
	}

	virtual FVector2D ComputeDesiredSize(float) const override;

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, 
						  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;


	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	//virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void SetOwner(class UMMoInputTouch* InputTouch) { this->Owner = InputTouch; }

protected:
	void HandlePress();
	void HandleInput();

protected:
	FOnTouchDown OnTouchDown;
	FOnTouchUp OnTouchUp;
	FOnTouchMove OnTouchMove;
private:
	class UMMoInputTouch* Owner;


	uint8 CurrentNumOfTouches;
	

	FVector2D CorrectedVisualSize;

	FVector2D CurrentLocalCoord;
	FVector2D PrevLocalCoord;	
	bool bHandleEvent;
	float TouchedPassed;
	bool bTouched;
	bool bNotifyTouchDown;
	bool bNotifyTouchUp;
	int8 CapturedPointerIndex;
	//FVector2D VisualCenter;
	FVector2D CorrectedCenter;

	//float PrevScaleFactor;
};
// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "Runtime/SlateCore/Public/Widgets/SLeafWidget.h"
#include "Events.h"
#include "Widgets/DeclarativeSyntaxSupport.h" 
#include "Runtime/SlateCore/Public/Styling/SlateTypes.h"

class MMOINPUT_API SMMoInputButton : public SLeafWidget {

public:
	SLATE_BEGIN_ARGS(SMMoInputButton)
	{}
	SLATE_EVENT(FOnTouchDown, OnTouchDown)
	SLATE_EVENT(FOnTouchUp, OnTouchUp)
	SLATE_EVENT(FOnTouchMove, OnTouchMove)
	SLATE_END_ARGS()

	// Called by UMMoInputButton::RebuildWidget
	void Construct(const FArguments& InArgs);

	/* Overwritten from SWidget, Called to tell a widget to paint itself(and it's children)
	 * 
	 * @param Args					All the arguments necessary to paint this widget (@todo umg: move all params into this struct)
	 * @param AllottedGeometry		The FGeometry that describes an area in which the widget should appear.
	 * @param MyCullingRect			The clipping rectangle allocated for this widget and its children.
	 * @param OutDrawElements		A list of FDrawElements to populate with the output.
	 * @param LayerId				The Layer onto which this widget should be rendered.
	 * @param InColorAndOpacity		Color and Opacity to be applied to all the descendants of the widget being painted
	 * @param bParentEnabled		True if the parent of this widget is enabled.
	 * @return The maximum layer ID attained by this widget or any of its children.
	*/
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
						  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	
	// Overwritten from SWidget
	virtual FVector2D ComputeDesiredSize(float) const override;

	/**
	 * Called when a touchpad touch is started (finger down)
	 *
	 * @param InTouchEvent The touch event generated
	 */
	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	/**
	 * Called when a touchpad touch is moved  (finger moved)
	 *
	 * @param InTouchEvent The touch event generated
	 */
	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	/**
	 * Called when a touchpad touch is ended (finger lifted)
	 *
	 * @param InTouchEvent The touch event generated
	 */
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void SetOwner(class UMMoInputButton* InputButton) { this->Owner = InputButton; }

	void SetIcon(const TAttribute<const FSlateBrush*>& InIcon);

	void SetDefault(const TAttribute<const FSlateBrush*>& InDefault);

	void SetPressed(const TAttribute<const FSlateBrush*>& InPressed);

	void SetRange(const TAttribute<const FSlateBrush*>& InRange);

	void SetTop(const TAttribute<const FSlateBrush*>& InTop);
protected:
	// Calculate the position of the finger so that it stays within the range of the button
	void HandleTouch(const FVector2D& LocalCoord);

	// Notify Blueprint
	void HandlePress();

	// Notify Blueprint
	void HandleInput();

protected:
	FInvalidatableBrushAttribute Icon;
	FInvalidatableBrushAttribute Default;
	FInvalidatableBrushAttribute Pressed;
	FInvalidatableBrushAttribute Range;
	FInvalidatableBrushAttribute Top;

	FOnTouchDown OnTouchDown;
	FOnTouchUp OnTouchUp;
	FOnTouchMove OnTouchMove;


private:
	class UMMoInputButton* Owner;
	bool bTouched;
	bool bNotifyTouchDown;
	bool bNotifyTouchUp;
	bool bHandleEvent;
	int8 CapturedPointerIndex;
	FVector2D CurrentLocalCoord;
	FVector2D PrevLocalCoord;
	float  PrevScaleFactor;

	FVector2D Default_CorrectedCenter;
	FVector2D Default_CorrectedVisualSize;
	FVector2D Icon_CorrectedVisualSize;

	FVector2D Range_VisualCenter;
	FVector2D Range_CorrectedVisualSize;

	FVector2D Top_Location;
	FVector2D Top_CorrectedVisualSize;
	bool bFirstMove;
};
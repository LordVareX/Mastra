// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "Runtime/SlateCore/Public/Widgets/SLeafWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h" 
#include "Runtime/SlateCore/Public/Styling/SlateTypes.h"

UENUM()
enum class EJoyStickState : uint8 {
	Inactivated,
	Activated,
}; 


class MMOINPUT_API SMMoInputJoystick : public SLeafWidget {

public:
	SLATE_BEGIN_ARGS(SMMoInputJoystick)
		: _bNeedFadeOut(false)
		, _FadeOutColor(FLinearColor(1.f,1.f,1.f,0.3f))
		, _bNeedRepositioning(false)
		, _bNeedReset(false)
		, _ResetDelay(1.f)
		, _RepositioningSize(FVector2D(200.f,200.f))
		, _bRepositioningWithinSlot(false)
		{}
	SLATE_ARGUMENT(bool, bNeedFadeOut)
	SLATE_ARGUMENT(FLinearColor, FadeOutColor)
	SLATE_ARGUMENT(bool, bNeedRepositioning)
	SLATE_ARGUMENT(bool, bNeedReset)
	SLATE_ARGUMENT(float, ResetDelay)
	SLATE_ARGUMENT(FVector2D, RepositioningSize)
	SLATE_ARGUMENT(bool, bRepositioningWithinSlot)
	SLATE_END_ARGS()

	// Called by UMMoInputJoystick::RebuildWidget
	void Construct(const FArguments& InArgs);
	
	/* Overwritten from SWidget, Called to tell a widget to paint itself(and it's children)
	*
	* @param Args					All the arguments necessary to paint this widget
	* @param AllottedGeometry		The FGeometry that describes an area in which the widget should appear.
	* @param MyCullingRect			The clipping rectangle allocated for this widget and its children.
	* @param OutDrawElements		A list of FDrawElements to populate with the output.
	* @param LayerId				The Layer onto which this widget should be rendered.
	* @param InColorAndOpacity		Color and Opacity to be applied to all the descendants of the widget being painted
	* @param bParentEnabled			True if the parent of this widget is enabled.
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

	void SetOwner(class UMMoInputJoystick* InputJoystick) { this->Owner = InputJoystick; }

	void SetRange(const TAttribute<const FSlateBrush*>& InRange);

	void SetTop(const TAttribute<const FSlateBrush*>& InTop);
protected:
	void HandleTouch(const FVector2D& LocalCoord);

	FLinearColor GetCurrentColor() const;
	
	FInvalidatableBrushAttribute Range;
	FInvalidatableBrushAttribute Top;
private:
	class UMMoInputJoystick* Owner;
	bool bHandleEvent;
	int8 CapturedPointerIndex;
	float PrevScaleFactor;

	FVector2D Range_CorrectedCenter;
	FVector2D Range_VisualCenter;
	FVector2D Range_CorrectedVisualSize;

	FVector2D Top_Location;
	FVector2D Top_CorrectedVisualSize;

	EJoyStickState CurrentState;
	FLinearColor CurrentColor;
	bool bNeedFade;
	FLinearColor bTargetFadeColor;
	bool bFading;
	float FadingDeltaTime;
	bool bResetting;
	float ResettingDeltaTime;
	FVector2D RepositioningSize;
	bool bRepositioningWithinSlot;
	bool bNeedReset;
	//FVector2D RepositioningPosition;
};
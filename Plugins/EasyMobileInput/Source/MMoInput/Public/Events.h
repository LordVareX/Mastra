// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once
#include "CoreMinimal.h"

// common touch events
DECLARE_DELEGATE_OneParam(FOnTouchDown, FVector2D /*Screen Position*/);
DECLARE_DELEGATE_OneParam(FOnTouchUp, FVector2D /*Screen Position*/);
DECLARE_DELEGATE_OneParam(FOnTouchMove, FVector2D /*Screen Position*/);

static void AlignBoxIntoScreen(FVector2D& Position, const FVector2D& Size, const FVector2D& ScreenSize)
{
	if (Size.X > ScreenSize.X || Size.Y > ScreenSize.Y)
	{
		return;
	}

	// Align box to fit into screen
	if (Position.X - Size.X * 0.5f < 0.f)
	{
		Position.X = Size.X * 0.5f;
	}

	if (Position.X + Size.X * 0.5f > ScreenSize.X)
	{
		Position.X = ScreenSize.X - Size.X * 0.5f;
	}

	if (Position.Y - Size.Y * 0.5f < 0.f)
	{
		Position.Y = Size.Y * 0.5f;
	}

	if (Position.Y + Size.Y * 0.5f > ScreenSize.Y)
	{
		Position.Y = ScreenSize.Y - Size.Y * 0.5f;
	}
}

static FORCEINLINE float GetScaleFactor(const FGeometry& Geometry) {
	const float DesiredWidth = 1024.0f;

	float UndoDPIScaling = 1.0f / Geometry.Scale;
	return (Geometry.GetDrawSize().GetMax() / DesiredWidth) * UndoDPIScaling;
}


static int32 ResolveRelativePosition(float Position, float RelativeTo, float ScaleFactor) {
	// absolute from edge
	if (Position < -1.0f)
	{
		return RelativeTo + Position * ScaleFactor;
	}
	// relative from edge
	else if (Position < 0.0f)
	{
		return RelativeTo + Position * RelativeTo;
	}
	// relative from 0
	else if (Position <= 1.0f)
	{
		return Position * RelativeTo;
	}
	// absolute from 0
	else
	{
		return Position * ScaleFactor;
	}
}

/**
Summary: Function that determine whether the touch position is within the range of the button

Parameters:
	ButtonCenter:	Center of button
	TouchPosition:	Position of touch
	BoxSize:		Button Size
	bRoundButton:	True if it is a round button

Return : True is within the range of the button
*/
static bool PositionIsInside(const FVector2D& InButtonCenter, const FVector2D& InTouchPosition, const FVector2D& InBoxSize, bool bRoundButton) {
	
	// normal button
	if (!bRoundButton) {
		return
			InTouchPosition.X >= InButtonCenter.X - InBoxSize.X * 0.5f &&
			InTouchPosition.X <= InButtonCenter.X + InBoxSize.X * 0.5f &&
			InTouchPosition.Y >= InButtonCenter.Y - InBoxSize.Y * 0.5f &&
			InTouchPosition.Y <= InButtonCenter.Y + InBoxSize.Y * 0.5f;
	}
	
	// round button
	float Radius = FMath::Min(InBoxSize.X, InBoxSize.Y) / 2.f;		// take the length of the smallest side as the radius
	return FVector2D::Distance(InTouchPosition, InButtonCenter) <= Radius;
	
}
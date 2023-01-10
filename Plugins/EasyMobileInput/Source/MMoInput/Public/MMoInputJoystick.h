// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "MMoInputJoystick.generated.h"

UCLASS()
class MMOINPUT_API UMMoInputJoystick : public UWidget {
	GENERATED_BODY()
public:
	UMMoInputJoystick();
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

public:
	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "horizontal axis"))
	FKey MainInputKey;

	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "vertical axis"))
	FKey AltInputKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Joystick Size", Category = "Control", meta = (ToolTip = "size of range"))
	FVector2D RangeSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Thumb Size", Category = "Control", meta = (ToolTip = "size of thumb"))
	FVector2D TopSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Background Image", Category = "Control", meta = (ToolTip = "range brush for joystick"))
	FSlateBrush Range;

	UPROPERTY()
	FGetSlateBrush RangeDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Thumb Image", Category = "Control", meta = (ToolTip = "thumb brush for joystick"))
	FSlateBrush Top;

	UPROPERTY()
	FGetSlateBrush TopDelegate;

	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "weather make joystick fade out"))
	bool bNeedFadeOut;
	
	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "the seconds before joystick fade out"))
	float FadeOutDelay;

	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "the fade out speed"))
	float FadeOutSpeed;

	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "the fade out color"))
	FLinearColor FadeOutColor;
	
	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "weather repositioning on touch"))
    bool bNeedRepositioning;

	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "weather repositioning within canvas panel slot"))
	bool bRepositioningWithinCanvasPanelSlot;

	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "weather reset joystick position"))
	bool bNeedResetPosition;
	
	UPROPERTY(EditAnywhere, Category = "Control", meta =(ToolTip = "the seconds before joystick reset"))
	float ResetPositionDelay;

	UFUNCTION(BlueprintCallable, Category = "Control")
	UMaterialInstanceDynamic* GetDynamicMaterialOfTop();

	UFUNCTION(BlueprintCallable, Category = "Control")
	UMaterialInstanceDynamic* GetDynamicMaterialOfRange();

	FVector2D GetRepositioningSize() const;
protected:
	TSharedPtr<class SMMoInputJoystick> MyJoystick;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	const FSlateBrush* ConvertRangeImage(TAttribute<FSlateBrush> InImageAsset) const;
	const FSlateBrush* ConvertTopImage(TAttribute<FSlateBrush> InImageAsset) const;
};
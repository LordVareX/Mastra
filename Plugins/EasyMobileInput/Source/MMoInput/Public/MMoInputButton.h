// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "MMoInputButton.generated.h"

UCLASS()
class MMOINPUT_API UMMoInputButton : public UWidget {
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchDownEvent, FVector2D, ScreenPosition);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchUpEvent, FVector2D, ScreenPosition);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchMoveEvent, FVector2D, ScreenPosition);

	UMMoInputButton();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
public:

	// Whether to draw the joystick
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "WithJoystick", Category = "Control", meta = (ToolTip = "need joystick"))
	bool bWithJoystick;
	
	// Button appearance when button is not pressed (Texture or Material)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control|Button", meta = (ToolTip = "normal brush for button"))
	FSlateBrush Default;

	UPROPERTY()
	FGetSlateBrush DefaultDelegate;

	// Button appearance when button is pressed (Texture or Material)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control|Button", meta = (ToolTip = "pressed brush for button"))
	FSlateBrush Pressed;

	UPROPERTY()
	FGetSlateBrush PressedDelegate;

	// Icon (Texture or Material)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control|Button", meta = (ToolTip = "material icon brush for button"))
	FSlateBrush Icon;

	UPROPERTY()
	FGetSlateBrush IconDelegate;

	// Get the dynamic material of the icon
	UFUNCTION(BlueprintCallable, Category = "Control|Button")
	UMaterialInstanceDynamic* GetDynamicMaterialOfIcon();

	// Get the dynamic material of the appearance when the button is not pressed
	UFUNCTION(BlueprintCallable, Category = "Control|Button")
	UMaterialInstanceDynamic* GetDynamicMaterialOfDefault();

	// Get the dynamic material of the appearance when the button is pressed
	UFUNCTION(BlueprintCallable, Category = "Control|Button")
	UMaterialInstanceDynamic* GetDynamicMaterialOfPressed();

	// Whether it is a round button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Round button", Category = "Control|Button", meta = (ToolTip = "whether it is a round button"))
	bool bCircular;
	
	// Size of button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control|Button", meta = (ToolTip = "size of button"))
	FVector2D ButtonSize;

	// Size of icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control|Button", meta = (ToolTip = "size of icon"))
	FVector2D IconSize;

	// The brush of background image of the joystick
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Background Image", Category = "Control|Joystick", meta = (ToolTip = "the brush of background image of the joystick"))
	FSlateBrush Range;

	UPROPERTY()
	FGetSlateBrush RangeDelegate;

	// The brush of the thumb image of the joystick
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Thumb Image", Category = "Control|Joystick", meta = (ToolTip = "the brush of the thumb image of the joystick"))
	FSlateBrush Top;

	UPROPERTY()
	FGetSlateBrush TopDelegate;

	// The size of the background image of the joystick
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Joystick Size", Category = "Control|Joystick", meta = (ToolTip = "the size of the background image of the joystick"))
	FVector2D RangeSize;

	// The size of the thumb image of the joystick
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Thumb Size", Category = "Control|Joystick", meta = (ToolTip = "the size of the thumb image of the joystick"))
	FVector2D TopSize;

	UPROPERTY(BlueprintAssignable, Category = "Control|Event")
	FOnTouchDownEvent OnTouchDown;

	UPROPERTY(BlueprintAssignable, Category = "Control|Event")
	FOnTouchUpEvent OnTouchUp;

	UPROPERTY(BlueprintAssignable, Category = "Control|Event")
	FOnTouchMoveEvent OnTouchMove;

	// Center of Button
	FVector2D Center; 

protected:
	void HandleOnTouchDown(FVector2D Location);
	void HandleOnTouchUp(FVector2D Location);
	void HandleOnTouchMove(FVector2D Location);

	const FSlateBrush* ConvertIconImage(TAttribute<FSlateBrush> InImageAsset) const;
	const FSlateBrush* ConvertDefaultImage(TAttribute<FSlateBrush> InImageAsset) const;
	const FSlateBrush* ConvertPressedImage(TAttribute<FSlateBrush> InImageAsset) const;
	const FSlateBrush* ConvertRangeImage(TAttribute<FSlateBrush> InImageAsset) const;
	const FSlateBrush* ConvertTopImage(TAttribute<FSlateBrush> InImageAsset) const;
protected:
	TSharedPtr<class SMMoInputButton> MyButton;
};
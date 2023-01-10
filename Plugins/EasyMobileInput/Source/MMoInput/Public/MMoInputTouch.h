// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "MMoInputTouch.generated.h"

UCLASS()
class MMOINPUT_API UMMoInputTouch : public UWidget {
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchDownEvent, FVector2D, Position);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchUpEvent, FVector2D, Position);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchMoveEvent, FVector2D, Position);

	UMMoInputTouch();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
	void HandleOnTouchDown(FVector2D Location);
	void HandleOnTouchUp(FVector2D Location);
	void HandleOnTouchMove(FVector2D Location);
public:
	TSharedPtr<ISlateBrushSource> Image1;
	
	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "is circular"))
	bool bCircular;

	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "background"))
	UTexture2D* BackGround;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control", meta = (ToolTip = "control is active"))
	FLinearColor ActiveColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control", meta = (ToolTip = "no control is active"))
	FLinearColor DeActiveColor;

	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "when pressed"))
	FKey PressKey;

	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "horizontal axis"))
	FKey MainInputKey;

	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "vertical axis"))
	FKey AltInputKey;

	UPROPERTY(BlueprintAssignable, Category = "Control|Event")
	FOnTouchDownEvent OnTouchDown;

	UPROPERTY(BlueprintAssignable, Category = "Control|Event")
	FOnTouchUpEvent OnTouchUp;

	UPROPERTY(BlueprintAssignable, Category = "Control|Event")
	FOnTouchMoveEvent OnTouchMove;

	FVector2D Center;


protected:
	TSharedPtr<class SMMoInputTouch> MyTouch;
};
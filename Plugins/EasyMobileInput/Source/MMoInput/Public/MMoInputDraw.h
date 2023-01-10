// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "CoreMinimal.h"
#include "MMoInputDraw.generated.h"

UCLASS(BlueprintType)
class MMOINPUT_API UMMoInputDraw : public UObject {
	GENERATED_BODY()

public:
	UMMoInputDraw();
	

	/* Function that draw the the scope of skill
	* 
	* InActor: The actor as the center of the circle
	* LocalOffset: Screen position
	* RangeOutRadius: Maximum radius
	* RangeInRadius: Effective radius
	* InRangeIn: The material representing the effective range
	* InRangeOut: The material representing the maximum range 
	* 
	* OutResultWorld: World position of the center of the effective range
	*/
	UFUNCTION(BlueprintCallable, Category = "MMo Input")
	UPARAM(DisplayName = "Ok") bool DrawRange(UPARAM(DisplayName = "FollowedTarget") AActor* InActor, UPARAM(DisplayName = "ScreenPosition") const FVector2D& LocalOffset, UPARAM(DisplayName = "ScreenRadiusOfJoystick") float WidgetRadius, UPARAM(DisplayName = "WorldRadiusOfRangeOut") float RangeOutRadius, UPARAM(DisplayName = "WorldRadiusOfRangeIn") float RangeInRadius, UPARAM(DisplayName = "RangeIn") UMaterial* InRangeIn, UPARAM(DisplayName = "RangeOut") UMaterial* InRangeOut, UPARAM(DisplayName = "WorldPosition") FVector& OutResultWorld);

	UFUNCTION(BlueprintCallable, Category = "MMo Input")
	void CleanRange();

	/* Function that draw the the scope of skill
	*
	* InActor: The actor as the center of the circle
	* LocalOffset: Screen position
	* Radius: Maximum radius
	* InRangeIn: The material representing the effective range
	* InRangeOut: The material representing the maximum range
	* FixedArrowSize: The width of the arrow
	* 
	* OutResultWorld: World position of the center of the effective range
	*/
	UFUNCTION(BlueprintCallable, Category = "MMo Input")
	UPARAM(DisplayName = "Ok") bool DrawArrow(UPARAM(DisplayName = "FollowedTarget") AActor* InActor, UPARAM(DisplayName = "ScreenPosition") const FVector2D& LocalOffset, UPARAM(DisplayName = "WorldRadiusOfArrow") float Radius, UPARAM(DisplayName = "Arrow") UMaterial* InArrow, UPARAM(DisplayName = "RangeOut")  UMaterial* InRangeOut, float FixedArrowSize, UPARAM(DisplayName = "WorldPosition") FVector& OutResultWorld);

	UFUNCTION(BlueprintCallable, Category = "MMo Input")
	void CleanArrow();
	
protected:
	UPROPERTY()
	class USceneComponent* AttachPointComp;
	
	UPROPERTY()
	class UDecalComponent* ArrowComponent;

	UPROPERTY()
	class UDecalComponent* ArrowRangeOutComponent;

	UPROPERTY()
	class UDecalComponent* RangeInComponent;

	UPROPERTY()
	class UDecalComponent* RangeOutComponent;
};
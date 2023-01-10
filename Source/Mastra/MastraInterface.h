// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MastraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UMastraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTRA_API IMastraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void OnActivate(const FString& Killer, const FString& Pawned, int TotalHonor);

	UFUNCTION()
		virtual void ActivatePure(float a, float b, float c, float d, float e, float f, float g) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void LookUp(const FString& str);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void LookUpObject(const AActor* obj);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void CheckBool(bool check);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void CheckStringWithFloat(const FString& str, const float& Float, bool check);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MastraInputLibrary.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTRA_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void OnInteract(const FName& ItemName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void OnGetValue(class APlayerController* pc, const FItem& ItemValue);

	UFUNCTION()
		virtual FName GetName() = 0;
};

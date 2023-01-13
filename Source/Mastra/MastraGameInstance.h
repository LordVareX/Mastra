// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MastraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MASTRA_API UMastraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
		FString CharacterName;
};

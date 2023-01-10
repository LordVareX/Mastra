// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MastraInterface.h"
#include "MastraSkillComponent.generated.h"

class AMastraCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTRA_API UMastraSkillComponent : public UActorComponent, public IMastraInterface
{
	GENERATED_BODY()

protected:
	int SkillLevel = 0;

public:	
	// Sets default values for this component's properties
	UMastraSkillComponent(const FObjectInitializer& ObjectInitializer);

protected:


	UFUNCTION(BlueprintImplementableEvent, Category = "Execute")
		void ExecuteSkill(AMastraCharacter* OwningChar);

public:	
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivatePure(float a, float b, float c, float d, float e, float f, float g) override; //c++ only interface function

	void Execute(AMastraCharacter* OwningChar);

		
};

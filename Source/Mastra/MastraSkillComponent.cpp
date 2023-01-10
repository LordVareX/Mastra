// Fill out your copyright notice in the Description page of Project Settings.


#include "MastraSkillComponent.h"
#include "Engine.h"
#include "MastraCharacter.h"

// Sets default values for this component's properties
UMastraSkillComponent::UMastraSkillComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMastraSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMastraSkillComponent::ActivatePure(float a, float b, float c, float d, float e, float f, float g)
{
}

void UMastraSkillComponent::Execute(AMastraCharacter* OwningChar)
{
	//General executing functions
	if (OwningChar != nullptr)
	{
		ExecuteSkill(OwningChar);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Execute")));
	}
}


// Called every frame
void UMastraSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


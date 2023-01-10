// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "MastraInputLibrary.h"
#include "MastraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MASTRA_API UMastraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UMastraAnimInstance();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		bool bMoving = false;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Switcher = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		bool canAttack = false;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		float Speed = 0.0f;

	UPROPERTY(Replicated, BlueprintReadWrite)
		EStyle AnimStyle;

protected:

	APawn* Owner;

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};

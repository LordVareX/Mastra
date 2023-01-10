// Fill out your copyright notice in the Description page of Project Settings.


#include "MastraAnimInstance.h"
#include "MastraCharacter.h"

void UMastraAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMastraAnimInstance, Speed);
	DOREPLIFETIME(UMastraAnimInstance, bMoving);
	DOREPLIFETIME(UMastraAnimInstance, CanMove);
	DOREPLIFETIME(UMastraAnimInstance, canAttack);
	DOREPLIFETIME(UMastraAnimInstance, AnimStyle);
}

UMastraAnimInstance::UMastraAnimInstance()
{

}

void UMastraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = TryGetPawnOwner();
}

void UMastraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*if (!Owner)
	{
		return;
	}

	else if (Owner->IsA(AMastraCharacter::StaticClass()))
	{
		AMastraCharacter* pc = Cast<AMastraCharacter>(Owner);

		if (pc)
		{
			isBox = pc->switchBox;
			isShao = pc->switchShao;
		}
	}*/
}

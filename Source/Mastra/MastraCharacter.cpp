// Copyright Epic Games, Inc. All Rights Reserved.

#include "MastraCharacter.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Styling/SlateColor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkinnedMeshComponent.h"
#include "Math/Rotator.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Materials/Material.h"
#include "Engine/World.h"

//////////////////////////////////////////////////////////////////////////
// AMastraCharacter
#include "MastraInputLibrary.h"
#include "MastraAnimInstance.h"
#include "MastraPlayerController.h"
#include "MastraGameState.h"
#include "MastraGameInstance.h"
#include "MastraPlayerState.h"
#include "MastraGameMode.h"
#include "MastraSkillComponent.h"

void AMastraCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMastraCharacter, Team);
	DOREPLIFETIME(AMastraCharacter, PlayerName);
	DOREPLIFETIME(AMastraCharacter, Level);
	DOREPLIFETIME(AMastraCharacter, Health);
	DOREPLIFETIME(AMastraCharacter, MaxHealth);
	DOREPLIFETIME(AMastraCharacter, HPRegen);
	DOREPLIFETIME(AMastraCharacter, PhysicalAttack);
	DOREPLIFETIME(AMastraCharacter, MagicPower);
	DOREPLIFETIME(AMastraCharacter, PhysicalDefense);
	DOREPLIFETIME(AMastraCharacter, MagicDefense);
	DOREPLIFETIME(AMastraCharacter, PhysicalPenetration);
	DOREPLIFETIME(AMastraCharacter, MagicalPenetration);
	DOREPLIFETIME(AMastraCharacter, DamageReduction);
	DOREPLIFETIME(AMastraCharacter, AttackSpeed);
	DOREPLIFETIME(AMastraCharacter, AttackSpeedRatio);
	DOREPLIFETIME(AMastraCharacter, MovementSpeed);
	DOREPLIFETIME(AMastraCharacter, RespawnTime);
}

AMastraCharacter::AMastraCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void AMastraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//RefreshPlayerData();
	ServerSetupDetails();
}

void AMastraCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

float AMastraCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (this->GetLocalRole() == ROLE_Authority)
	{
		if (DamageCauser != this)
		{
			Health = Health - Damage;
		}
	}
	return 0.0f;
}

void AMastraCharacter::RefreshPlayerData()
{
	if (IsLocallyControlled())
	{
		ServerSetupDetails();
	}
}

bool AMastraCharacter::ServerSetupDetails_Validate()
{
	return true;
}

void AMastraCharacter::ServerSetupDetails_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("SERVERSETUPSTATS")));
	SetupDetails();
}

bool AMastraCharacter::SetupDetails_Validate()
{
	return true;
}

void AMastraCharacter::SetupDetails_Implementation()
{
	GI = Cast<UMastraGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GI != nullptr)
	{
		FCharacterBase* OutRow = CharacterTable->FindRow<FCharacterBase>(FName(*GI->CharacterName), "");
		if (OutRow)
		{
			this->GetMesh()->SetSkeletalMesh(OutRow->Character_Details.Character_Mesh);
			this->GetMesh()->SetAnimInstanceClass(OutRow->Character_Details.Character_AnimBP);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Assigned")));
			ServerSetupStats();
		}
	}
}

bool AMastraCharacter::ServerSetupStats_Validate()
{
	return true;
}

void AMastraCharacter::ServerSetupStats_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("SERVERSETUPSTATS")));
	SetupStats();
}

bool AMastraCharacter::SetupStats_Validate()
{
	return true;
}

void AMastraCharacter::SetupStats_Implementation()
{
	GI = Cast<UMastraGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GI != nullptr)
	{
		FCharacterBase* OutRow = CharacterTable->FindRow<FCharacterBase>(FName(*GI->CharacterName), "");
		if (OutRow)
		{
			for (int i = 0; i < OutRow->Character_Status.Num(); ++i)
			{
				OutRow->Character_Status[i];

				if (OutRow->Character_Status[i].Level == Level)
				{
					Health = OutRow->Character_Status[i].HP;
					MaxHealth = OutRow->Character_Status[i].HP;
					HPRegen = OutRow->Character_Status[i].HP_Regen;

					GetWorldTimerManager().SetTimer(HPRegenHandle, this, &AMastraCharacter::RegenHP, 0.1f, true, 1.0f);
				}
			}
		}
	}
}

void AMastraCharacter::RegenHP()
{
	Health = FMath::Clamp(Health + HPRegen, 0.0f, MaxHealth);
}

void AMastraCharacter::LevelIncreased()
{
	GI = Cast<UMastraGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GI != nullptr)
	{
		FCharacterBase* OutRow = CharacterTable->FindRow<FCharacterBase>(FName(*GI->CharacterName), "");
		if (OutRow)
		{
			for (int i = 0; i < OutRow->Character_Status.Num(); ++i)
			{
				OutRow->Character_Status[i];

				if (OutRow->Character_Status[i].Level == Level)
				{
					MaxHealth = OutRow->Character_Status[i].HP;
					HPRegen = OutRow->Character_Status[i].HP_Regen;
				}
			}
		}
	}
}
//bool AMastraCharacter::DetectNearestTarget_Validate(EResult Type, FActionSkill SelectedRow)
//{
//	return true;
//}



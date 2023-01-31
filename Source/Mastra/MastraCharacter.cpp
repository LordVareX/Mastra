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
	DOREPLIFETIME(AMastraCharacter, PickedCharacter);
	DOREPLIFETIME(AMastraCharacter, PlayerName);
	DOREPLIFETIME(AMastraCharacter, Level);
	DOREPLIFETIME(AMastraCharacter, Health);
	DOREPLIFETIME(AMastraCharacter, MaxHealth);
	DOREPLIFETIME(AMastraCharacter, MaxHP);
	DOREPLIFETIME(AMastraCharacter, HPRegen);
	DOREPLIFETIME(AMastraCharacter, Mana);
	DOREPLIFETIME(AMastraCharacter, MaxMana);
	DOREPLIFETIME(AMastraCharacter, ManaRegen);
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
	DOREPLIFETIME(AMastraCharacter, SpawnTransform);
	DOREPLIFETIME(AMastraCharacter, CMesh);

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
	CameraBoom->SetUsingAbsoluteRotation(false); // Don't want arm to rotate when character does
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

	SetReplicates(true);
	SetReplicateMovement(true);

	MaxHP = MaxHealth;
}

void AMastraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnTransform = GetActorTransform();
	//RefreshPlayerData();
	//ServerSetupDetails();
	//UpdateUI(Health, MaxHealth, Level);
}

void AMastraCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMastraCharacter::OnRep_HealthUpdated()
{
	AMastraPlayerState* PS = Cast<AMastraPlayerState>(GetPlayerState());
	if (PS)
	{
		UpdateUI(Health);
	}
}

float AMastraCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (this->GetLocalRole() == ROLE_Authority)
	{
		if (DamageCauser != this)
		{
			AMastraPlayerState* PS = Cast<AMastraPlayerState>(GetPlayerState());
			if (PS)
			{
				Health = Health - Damage;
				//UpdateUI(Health);
				if (Health <= 0)
				{
					GetWorldTimerManager().ClearTimer(HPRegenHandle);
					//PS->StartRespawnTimer(PS);
					//this->Destroy();
					//this->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
					//GetWorldTimerManager().SetTimer(RespawnHandle, this, &AMastraCharacter::RespawnCharacter, 5.0f, false);
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("Respawn")));
					RespawnUpdate();
				}
			}
		}
	}
	return Health;
}

bool AMastraCharacter::RespawnCharacter_Validate()
{
	return true;
}

void AMastraCharacter::RespawnCharacter_Implementation()
{
	AMastraPlayerController* PC = Cast<AMastraPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		//Set current input to interact with UI in spectator mode
		/*if (PC->IsLocalPlayerController() && PC->GetNetMode() != ENetMode::NM_DedicatedServer)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
		}*/
		AMastraPlayerState* PS = Cast<AMastraPlayerState>(PC->PlayerState);
		if (PS)
		{
			PC->RespawnPawn(PS->SpawnTransform);
			PC->UnPossess();
		}
	}
}

void AMastraCharacter::RefreshPlayerData()
{
	//if (IsLocallyControlled())
	//{
		ServerSetupDetails("Ratna");
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("%s"), *PickedCharacter));
	//}
}

bool AMastraCharacter::ServerLocked_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::ServerLocked_Implementation(const FString& CharacterName)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("SERVERSETUPSTATS")));
	Locked(CharacterName);
}

bool AMastraCharacter::Locked_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::Locked_Implementation(const FString& CharacterName)
{
	
	ServerSetupDetails(CharacterName);
	PickedCharacter = CharacterName;
	//UpdateUI(Health, MaxHealth, Level);
}

bool AMastraCharacter::ServerSetupDetails_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::ServerSetupDetails_Implementation(const FString& CharacterName)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("SERVERSETUPSTATS")));
	SetupDetails(CharacterName);
}

bool AMastraCharacter::SetupDetails_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::SetupDetails_Implementation(const FString& CharacterName)
{
	GI = Cast<UMastraGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GI != nullptr)
	{
		FCharacterBase* OutRow = CharacterTable->FindRow<FCharacterBase>(FName(*CharacterName), "");//(FName(*GI->CharacterName), "");
		if (OutRow)
		{
			CMesh = OutRow->Character_Details.Character_Mesh;
			CAnimBP = OutRow->Character_Details.Character_AnimBP;
			this->GetMesh()->SetSkeletalMesh(OutRow->Character_Details.Character_Mesh);
			this->GetMesh()->SetAnimInstanceClass(OutRow->Character_Details.Character_AnimBP);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Assigned")));
			ServerSetupStats(CharacterName);
		}
	}
}

bool AMastraCharacter::ServerSetupStats_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::ServerSetupStats_Implementation(const FString& CharacterName)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("SERVERSETUPSTATS")));
	SetupStats(CharacterName);

	
}

bool AMastraCharacter::SetupStats_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::SetupStats_Implementation(const FString& CharacterName)
{
	AMastraPlayerState* PS = Cast<AMastraPlayerState>(GetPlayerState());

	if (PS)
	{
		FCharacterBase* OutRow = CharacterTable->FindRow<FCharacterBase>(FName(*CharacterName), "");
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
					Mana = OutRow->Character_Status[i].Mana;
					MaxMana = OutRow->Character_Status[i].Mana;
					ManaRegen = OutRow->Character_Status[i].Mana_Regen;
					PhysicalAttack = OutRow->Character_Status[i].Physical_Attack;
					MagicPower = OutRow->Character_Status[i].Magic_Power;
					PhysicalDefense = OutRow->Character_Status[i].Physical_Defense;
					MagicDefense = OutRow->Character_Status[i].Magic_Defense;
					PhysicalPenetration = OutRow->Character_Status[i].Physical_Penetration;
					MagicalPenetration = OutRow->Character_Status[i].Magic_Penetration;
					DamageReduction = OutRow->Character_Status[i].Damage_Reduction;
					AttackSpeed = OutRow->Character_Status[i].Attack_Speed;
					AttackSpeedRatio = OutRow->Character_Status[i].Attack_Speed_Ratio;
					MovementSpeed = OutRow->Character_Status[i].Movement_Speed;

					PS->Health = Health;
					PS->MaxHealth = MaxHealth;
					PS->HPRegen = HPRegen;
					PS->Mana = Mana;
					PS->MaxMana = MaxMana;
					PS->ManaRegen = ManaRegen;
					PS->PhysicalAttack = PhysicalAttack;
					PS->MagicPower = MagicPower;
					PS->PhysicalDefense = PhysicalDefense;
					PS->MagicDefense = MagicDefense;
					PS->PhysicalPenetration = PhysicalPenetration;
					PS->MagicalPenetration = MagicalPenetration;
					PS->DamageReduction = DamageReduction;
					PS->AttackSpeed = AttackSpeed;
					PS->AttackSpeedRatio = AttackSpeedRatio;
					PS->MovementSpeed = MovementSpeed;
					
					//RegenHP();
					OnRep_HealthUpdated();
					//RegenHealth(MaxHealth);
					UpdateStats(MaxHealth, Level);

					GetWorldTimerManager().SetTimer(HPRegenHandle, this, &AMastraCharacter::RegenHP, 0.1f, true, 0.1f);
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Health : %f"), Health));
					
				}
			}
		}
	}
}


bool AMastraCharacter::ServerLevelIncreased_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::ServerLevelIncreased_Implementation(const FString& CharacterName)
{

	LevelIncreased(CharacterName);
	
}

bool AMastraCharacter::LevelIncreased_Validate(const FString& CharacterName)
{
	return true;
}

void AMastraCharacter::LevelIncreased_Implementation(const FString& CharacterName)
{
		AMastraPlayerState* PS = Cast<AMastraPlayerState>(GetPlayerState());

		if (PS)
		{
			FCharacterBase* OutRow = CharacterTable->FindRow<FCharacterBase>(FName(*CharacterName), "");
			if (OutRow)
			{
				for (int i = 0; i < OutRow->Character_Status.Num(); ++i)
				{
					OutRow->Character_Status[i];

					if (OutRow->Character_Status[i].Level == Level)
					{
						MaxHealth = OutRow->Character_Status[i].HP;
						HPRegen = OutRow->Character_Status[i].HP_Regen;
						MaxMana = OutRow->Character_Status[i].Mana;
						ManaRegen = OutRow->Character_Status[i].Mana_Regen;
						PhysicalAttack = OutRow->Character_Status[i].Physical_Attack;
						MagicPower = OutRow->Character_Status[i].Magic_Power;
						PhysicalDefense = OutRow->Character_Status[i].Physical_Defense;
						MagicDefense = OutRow->Character_Status[i].Magic_Defense;
						PhysicalPenetration = OutRow->Character_Status[i].Physical_Penetration;
						MagicalPenetration = OutRow->Character_Status[i].Magic_Penetration;
						DamageReduction = OutRow->Character_Status[i].Damage_Reduction;
						AttackSpeed = OutRow->Character_Status[i].Attack_Speed;
						AttackSpeedRatio = OutRow->Character_Status[i].Attack_Speed_Ratio;
						MovementSpeed = OutRow->Character_Status[i].Movement_Speed;
						//RegenHP();
						UpdateStats(MaxHealth, Level);
						
		

						//UpdateUI(Health);
						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Health : %f"), MaxHealth));
						
					}
				}
			}
		}
	
}




void AMastraCharacter::RegenHP()
{
	//if (this->GetLocalRole() == ROLE_Authority)
	//{
		Health = FMath::Clamp(Health + HPRegen, 0.0f, MaxHealth);
	//}
	
}

//bool AMastraCharacter::DetectNearestTarget_Validate(EResult Type, FActionSkill SelectedRow)
//{
//	return true;
//}



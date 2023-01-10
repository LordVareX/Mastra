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
//#include "DestructibleTower.h"
#include "MastraGameState.h"
#include "MastraPlayerState.h"
#include "MastraGameMode.h"
//#include "BMobaTriggerCapsule.h"
//#include "MastraCTF.h"
#include "MastraSkillComponent.h"

void AMastraCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AMastraCharacter, TeamName);
	DOREPLIFETIME(AMastraCharacter, PlayerName);
	//DOREPLIFETIME(AMastraCharacter, Health);
	DOREPLIFETIME(AMastraCharacter, Stamina);
	DOREPLIFETIME(AMastraCharacter, IsHit);
	DOREPLIFETIME(AMastraCharacter, InRagdoll);
	DOREPLIFETIME(AMastraCharacter, BoneName);
	DOREPLIFETIME(AMastraCharacter, HitLocation);
	DOREPLIFETIME(AMastraCharacter, AttackSection);
	DOREPLIFETIME(AMastraCharacter, DamageDealers);
	DOREPLIFETIME(AMastraCharacter, Rotate);
	DOREPLIFETIME(AMastraCharacter, AttackerLocation);
	DOREPLIFETIME(AMastraCharacter, CharMesh);
	DOREPLIFETIME(AMastraCharacter, currentTarget);
	DOREPLIFETIME(AMastraCharacter, CounterMoveset);
	DOREPLIFETIME(AMastraCharacter, CTFteam);
	DOREPLIFETIME(AMastraCharacter, CTFentering);
	DOREPLIFETIME(AMastraCharacter, ActorsToGetGold);
	DOREPLIFETIME(AMastraCharacter, closestActor);
	DOREPLIFETIME(AMastraCharacter, RotateToActor);
	DOREPLIFETIME(AMastraCharacter, IsStunned);
	DOREPLIFETIME(AMastraCharacter, OnSpecialAttack);
	DOREPLIFETIME(AMastraCharacter, ActiveColliders);
	DOREPLIFETIME(AMastraCharacter, ArrDamagedEnemy);
	DOREPLIFETIME(AMastraCharacter, comboCount);
	DOREPLIFETIME(AMastraCharacter, MaxHealth);
	DOREPLIFETIME(AMastraCharacter, Defense);
	DOREPLIFETIME(AMastraCharacter, StunDuration);
	DOREPLIFETIME(AMastraCharacter, StunImpulse);
	DOREPLIFETIME(AMastraCharacter, FrontHitMoveset);
	DOREPLIFETIME(AMastraCharacter, BackHitMoveset);
	DOREPLIFETIME(AMastraCharacter, RightHitMoveset);
	DOREPLIFETIME(AMastraCharacter, LeftHitMoveset);
	DOREPLIFETIME(AMastraCharacter, SkillComp);
	DOREPLIFETIME(AMastraCharacter, ActorsInVision);
	DOREPLIFETIME(AMastraCharacter, IsCurrentlyVisible);
	DOREPLIFETIME(AMastraCharacter, damagedActor);
	DOREPLIFETIME(AMastraCharacter, IsImmuned);
	DOREPLIFETIME(AMastraCharacter, ImmunityDur);
	DOREPLIFETIME(AMastraCharacter, ActionSkillName);
	DOREPLIFETIME(AMastraCharacter, AtkSpeed);
	DOREPLIFETIME(AMastraCharacter, MoveSpeed);
	DOREPLIFETIME(AMastraCharacter, KnockbackVector);
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

	//WidgetComponent
	//W_DamageOutput = CreateDefaultSubobject<UWidgetComponent>(TEXT("W_DamageOutput"));
	//W_DamageOutput->SetupAttachment(RootComponent);
	//W_DamageOutput->SetRelativeLocation(FVector(0.000000f, 0.0f, 100.0f));
	//W_DamageOutput->InitWidget();
	//W_DamageOutput->SetIsReplicated(true);

	//W_DamageOutput->SetWidgetSpace(EWidgetSpace::Screen);
	//W_DamageOutput->SetDrawSize(FVector2D(500.0f, 500.0f));
	//W_DamageOutput->SetDrawAtDesiredSize(true);
	//W_DamageOutput->SetVisibility(false);
	//W_DamageOutput->SetGenerateOverlapEvents(false);

	TraceDistance = 20.0f;
}

void AMastraCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}


//void AMastraCharacter::OnRep_Health()
//{
//	UUserWidget* HPWidget = Cast<UUserWidget>(W_DamageOutput->GetUserWidgetObject());
//	if (HPWidget)
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Player %s with %s Widget"), *GetDebugName(this), *HPWidget->GetFName().ToString()));
//		const FName hptext = FName(TEXT("HealthText"));
//		UTextBlock* HealthText = (UTextBlock*)(HPWidget->WidgetTree->FindWidget(hptext));
//
//		const FName hpbar = FName(TEXT("HPBar"));
//		UProgressBar* HealthBar = (UProgressBar*)(HPWidget->WidgetTree->FindWidget(hpbar));
//
//		if (HealthText)
//		{
//			FString TheFloatStr = FString::SanitizeFloat(this->Health);
//
//			HealthText->SetText(FText::FromString(TheFloatStr));
//			HealthBar->SetPercent(FMath::Clamp(this->Health / this->MaxHealth, 0.0f, 1.0f));
//		}
//	}
//	//this->Health = UGestureInputsFunctions::UpdateProgressBarComponent(this->WidgetHUD, "HPBar", "Health", "HP", "Pain Meter", this->Health, this->MaxHealth);
//
//	/*if (this->IsLocallyControlled())
//	{
//		this->Health = UGestureInputsFunctions::UpdateProgressBarComponent(this->WidgetHUD, "HPBar", "Health", "HP", "Pain Meter", this->Health, this->MaxHealth);
//	}*/
//	/*if (!this->IsLocallyControlled())
//	{
//		float Health1 = UGestureInputsFunctions::UpdateProgressBarComponent(this->WidgetHUD, "HPBarMain_1", "Health_1", "HP", "Pain Meter", this->Health, this->MaxHealth);
//	}*/
//}

void AMastraCharacter::RefreshPlayerData()
{
	if (IsLocallyControlled())
	{
		ServerSetupStats();
	}

	//AMastraPlayerState* PS = Cast<AMastraPlayerState>(GetPlayerState());
	//if (PS)
	//{
	//	if (IsLocallyControlled())
	//	{
	//		ServerSetBlendspace(PS);
	//	}
	//}

	///*this->GetMesh()->SetSkeletalMesh(CharMesh, false);
	//AnimInsta = Cast<UMastraAnimInstance>(this->GetMesh()->GetAnimInstance());*/


	//FTimerHandle handle;
	//FTimerDelegate TimerDelegate;

	////set the row boolean to false after finish cooldown timer
	//TimerDelegate.BindLambda([this]()
	//{
	//	this->GetMesh()->SetVisibility(true);

		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Rotate: %s"), Rotate ? TEXT("true") : TEXT("false")));

		/*APlayerController* pc = Cast<APlayerController>(this->GetController());
		if (pc)
		{
			if (pc->IsLocalPlayerController() && pc->GetNetMode() != ENetMode::NM_DedicatedServer)
			{

				pc->bShowMouseCursor = false;
				pc->SetInputMode(FInputModeGameOnly());
			}
		}*/

	//});
	//this->GetWorldTimerManager().SetTimer(handle, TimerDelegate, 1.0f, false);

	//for (TActorIterator<AMastraCTF> It(GetWorld()); It; ++It)
	//{
	//	Towers.Add(*It);
	//}

	//CreateCPHUD();

	//FinishSetupBeginPlay();

	//if (IsLocallyControlled() == true)
	//{
	//	// Iterate over all actors, can also supply a different base class if needed
	//	for (TActorIterator<AMastraCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//	{
	//		// Follow iterator object to my actual actor pointer
	//		AMastraCharacter* MyActor = *ActorItr;
	//		if (MyActor->TeamName != this->TeamName)
	//		{
	//			MyActor->GetMesh()->SetCullDistance(1000.f);
	//			MyActor->Outline->SetCullDistance(1000.f);
	//		}
	//	}
	//}
}

//bool AMastraCharacter::ServerSetBlendspace_Validate(AMastraPlayerState* PS)
//{
//	return true;
//}
//
//void AMastraCharacter::ServerSetBlendspace_Implementation(AMastraPlayerState* PS)
//{
//	MultiSetBlendspace(PS);
//}
//
//bool AMastraCharacter::MultiSetBlendspace_Validate(AMastraPlayerState* PS)
//{
//	return true;
//}
//
//void AMastraCharacter::MultiSetBlendspace_Implementation(AMastraPlayerState* PS)
//{
//	if (PS)
//	{
//		UMastraAnimInstance* animinstance = Cast<UMastraAnimInstance>(this->GetMesh()->GetAnimInstance());
//		if (animinstance)
//		{
//			//Get int to enum
//			animinstance->AnimStyle = static_cast<EStyle>(PS->CurrentStyle);
//		}
//	}
//}

//void AMastraCharacter::OnRep_Team()
//{
//	UUserWidget* HPWidget = Cast<UUserWidget>(W_DamageOutput->GetUserWidgetObject());
//	if (HPWidget)
//	{
//		const FName hptext = FName(TEXT("TeamName"));
//		UTextBlock* HealthText = (UTextBlock*)(HPWidget->WidgetTree->FindWidget(hptext));
//
//		if (HealthText)
//		{
//			HealthText->SetText(FText::FromName(TeamName));
//		}
//	}
//}

//bool AMastraCharacter::ServerSetMaxWalkSpeed_Validate(float Val)
//{
//	return true;
//}
//
//void AMastraCharacter::ServerSetMaxWalkSpeed_Implementation(float Val)
//{
//	GetCharacterMovement()->MaxWalkSpeed = Val;
//}

bool AMastraCharacter::ServerSetupBaseStats_Validate(float HealthMax, float Def, float MvSpeed, float AttSpeed, float StunDur, float knockBack, float ImmDur)
{
	return true;
}

void AMastraCharacter::ServerSetupBaseStats_Implementation(float HealthMax, float Def, float MvSpeed, float AttSpeed, float StunDur, float knockBack, float ImmDur)
{
	SetupBaseStats(HealthMax, Def, MvSpeed, AttSpeed, StunDur, knockBack, ImmDur);
}

bool AMastraCharacter::SetupBaseStats_Validate(float HealthMax, float Def, float MvSpeed, float AttSpeed, float StunDur, float knockBack, float ImmDur)
{
	return true;
}

void AMastraCharacter::SetupBaseStats_Implementation(float HealthMax, float Def, float MvSpeed, float AttSpeed, float StunDur, float knockBack, float ImmDur)
{
	//float hpPercent = UMastraInputLibrary::CalculatePercentageFromValue(Health, MaxHealth, 100.0f);
	//Health = UMastraInputLibrary::CalculateValueFromPercentage(hpPercent, HealthMax, 100.0f);
	MaxHealth = HealthMax;
	Defence = Def;
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

//bool AMastraCharacter::ServerSetVisibility_Validate(AMastraCharacter* owningActor, AMastraCharacter* Actor, float MaxDrawDist, bool Entering)
//{
//	return true;
//}
//
//void AMastraCharacter::ServerSetVisibility_Implementation(AMastraCharacter* owningActor, AMastraCharacter* Actor, float MaxDrawDist, bool Entering)
//{
//	MulticastSetVisibility(owningActor, Actor, MaxDrawDist, Entering);
//}
//
//bool AMastraCharacter::MulticastSetVisibility_Validate(AMastraCharacter* owningActor, AMastraCharacter* Actor, float MaxDrawDist, bool Entering)
//{
//	return true;
//}
//
//void AMastraCharacter::MulticastSetVisibility_Implementation(AMastraCharacter* owningActor, AMastraCharacter* Actor, float MaxDrawDist, bool Entering)
//{
//	ActorsInVision.AddUnique(Actor);
//	UMastraInputLibrary::SetActorVisibility(Actor, ActorsInVision, MaxDrawDist, Entering, owningActor);
//}

void AMastraCharacter::SetupStats_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("SETUPSTATS")));
	AMastraPlayerState* PS = Cast<AMastraPlayerState>(GetPlayerState());
	if (PS)
	{
		AMastraPlayerController* pc = Cast<AMastraPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		//if (pc->MainWidget)
		//{
		//	MainWidget = pc->MainWidget;
		//}
		SetupBaseStats(PS->MaxHealth, PS->Defense, PS->MoveSpeed, PS->AtkSpeed, PS->StunDuration, PS->KnockbackVector, PS->ImmunityDur);
		CharMesh = PS->CharMesh;
		ActionTable = PS->ActionTable;
		//Health = MaxHealth;
		FrontHitMoveset = PS->FrontHitMoveset;
		BackHitMoveset = PS->BackHitMoveset;
		LeftHitMoveset = PS->LeftHitMoveset;
		RightHitMoveset = PS->RightHitMoveset;
		SkillComp = PS->SkillComponent;


		this->GetMesh()->SetSkeletalMesh(CharMesh, false);
		AnimInsta = Cast<UMastraAnimInstance>(this->GetMesh()->GetAnimInstance());

		//Remove previous skill component
		/*for (int32 i = 0; i < this->GetInstanceComponents().Num(); i++)
		{
			UMastraSkillComponent* SkillComponent = Cast<UMastraSkillComponent>(this->GetInstanceComponents()[i]);
			if (SkillComponent)
			{
				SkillComponent->DestroyComponent();
			}
		}*/

		if (ActionTable)
		{
			FString Context;
			for (auto& name : ActionTable->GetRowNames())
			{
				FActionSkill* row = ActionTable->FindRow<FActionSkill>(name, Context);

				if (row)
				{
					row->isOnCD = false;

					//Add skill component
					/*row->SkillComp = UInputLibrary::AddComponentByClass(row->SkillComponent, this);*/
				}
			}
		}
	}
}
//bool AMastraCharacter::DetectNearestTarget_Validate(EResult Type, FActionSkill SelectedRow)
//{
//	return true;
//}

void AMastraCharacter::Activate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Implementation"));
}

void AMastraCharacter::ActivatePure(float a, float b, float c, float d, float e, float f, float g)
{
	if (this->IsLocallyControlled())
	{
		this->ServerSetupBaseStats(a, b, c, d, e, f, g);
	}
	//if (MainWidget != nullptr && MainWidget->GetClass()->ImplementsInterface(UMastraInterface::StaticClass()))
	//{
		//Opens level up windows
	//	Cast<IMastraInterface>(MainWidget)->Execute_CheckBool(MainWidget, true);
	//}
	UE_LOG(LogTemp, Warning, TEXT("Pure"));
}
//
//void AMastraCharacter::CheckBool_Implementation(bool check)
//{
//	if (check)
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("EXIST")));
//		IsExist = true;
//	}
//}

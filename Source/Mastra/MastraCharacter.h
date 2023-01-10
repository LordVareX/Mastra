// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h" 
#include "MastraInputLibrary.h"
#include "GameFramework/Character.h"
#include "MastraAnimInstance.h"
#include "MastraInterface.h"
#include "MastraCharacter.generated.h"

UCLASS(config = Game)
class AMastraCharacter : public ACharacter, public IMastraInterface
{
	GENERATED_BODY()

		//Replicated Network setup
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* FogCol;


	private:
		bool IsExist = false;

		float Rad = 0.0f;

		//Get radius of visible area from sphere mask radius
		UMaterialParameterCollectionInstance* inst_Fog;

public:
	AMastraCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
		bool IsCurrentlyVisible = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LevelUp")
		bool IsUpgradingSkill = false;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status", Meta = (ExposeOnSpawn = "true"))
		FString PlayerName;

	//Setting up character mesh for player
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status", Meta = (ExposeOnSpawn = "true"))
		USkeletalMesh* CharMesh;

	////3D UI On Player's head
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	//	class UWidgetComponent* W_DamageOutput;

	//Outline
	//Setting up character mesh for player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, Meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Outline;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		TArray<class AMastraPlayerState*> DamageDealers;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "ControlFlag")
		bool CTFentering;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "ControlFlag")
		TArray<AActor*> ActorsToGetGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkillComponents")
		TArray<UMastraSkillComponent*> SkillComponents;

	//UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Health, BlueprintReadWrite, Category = "Status")
	//	float Health;

	//UFUNCTION()
	//	void OnRep_Health();

	//UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Team, BlueprintReadWrite, Category = "Status", Meta = (ExposeOnSpawn = "true"))
	//	FName TeamName;

	//UFUNCTION()
	//	void OnRep_Team();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



protected:

	//void OnHRMontageEnd(UAnimMontage* animMontage, bool bInterrupted);

	////Fog area overlap begin function
	//UFUNCTION()
	//	void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	////Fog area overlap end function
	//UFUNCTION()
	//	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	////Call out server to set visibility status of overlapping actor/s
	//UFUNCTION(Reliable, Server, WithValidation, Category = "VisibleOnFog")
	//	void ServerSetVisibility(AMastraCharacter* owningActor, AMastraCharacter* Actor, float MaxDrawDist, bool Entering);

	//UFUNCTION(Reliable, NetMulticast, WithValidation, Category = "VisibleOnFog")
	//	void MulticastSetVisibility(AMastraCharacter* owningActor, AMastraCharacter* Actor, float MaxDrawDist, bool Entering);

	//UFUNCTION(Reliable, Server, WithValidation, Category = "Movement")
	//	void ServerSetBlendspace(AMastraPlayerState* PS);

	//UFUNCTION(Reliable, NetMulticast, WithValidation, Category = "Movement")
	//	void MultiSetBlendspace(AMastraPlayerState* PS);

	//UFUNCTION(Reliable, Server, WithValidation, Category = "Movement")
	//	void ServerSetMaxWalkSpeed(float Val);

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
		void FinishSetupBeginPlay();

protected:

	//rain checks on action skills to be executed
	bool ActionEnabled = true;

	//Get overlapping actors from within FogCol
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "FogActors")
		TArray<AActor*> ActorsInVision;

	//Material Parameter Collection asset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UMaterialParameterCollection* Mat_Fog;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD", Meta = (ExposeOnSpawn = "true"))
	//	UUserWidget* MainWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		float TraceDistance = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
		int OrbsAmount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		float BaseDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int MinDamage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int MaxDamage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float ActualDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float BuffDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		float Defence = 110.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float BuffDefence = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float ReducedDefence = 0.0f;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "ActionSkill")
		UAnimMontage* CounterMoveset;

	//Assign actionskill data table from bp 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* ActionTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* HitReactionMoveset;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* FrontHitMoveset;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* BackHitMoveset;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* RightHitMoveset;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* LeftHitMoveset;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float StunDuration;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Status")
		float StunImpulse;

	//TimerHandle for removing damage dealer array
	FTimerHandle DealerTimer;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		bool IsHit;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Status")
		bool IsStunned = false;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Status")
		bool OnSpecialAttack = false;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Status")
		bool IsImmuned = false;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Status")
		float ImmunityDur = 0.0f;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "ActionSkill")
		FName ActionSkillName = "";

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Anim")
		bool InRagdoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
		bool DoOnce = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Rotate")
		bool Rotate = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Rotate")
		AActor* currentTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotate")
		bool test = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
		bool WithinVicinity = false;

	UPROPERTY(BlueprintReadOnly, Category = "Rotate")
		TArray<AActor*> FoundActors;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "HitReaction")
		FVector AttackerLocation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "HitReaction")
		FVector HitLocation;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
		FName BoneName = "pelvis";

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float Defense;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float Stamina;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "ActionSkill")
		FName AttackSection = "NormalAttack01";

	//		time interval between each normal attack combo
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float AtkSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float MoveSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float KnockbackVector = 0.0f;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "ActionSkill")
		int comboCount = 0;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "ActionSkill")
		bool OnComboDelay = false;

	UPROPERTY(VisibleAnywhere, Category = "ActionSkill")
		int slumberCount = 0;

	//*********************Knockout and Respawn***********************************//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Respawn")
		FTimerHandle RespawnTimer;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
		class UMastraAnimInstance* AnimInsta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HitReaction")
		FName ActiveSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "ControlFlag")
		FName CTFteam = "";

	UPROPERTY(EditAnywhere, Category = "Rotate")
		float RotateRadius = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Rotate")
		class AActor* closestActor;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Rotate")
		class AActor* damagedActor;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Rotate")
		class AMastraCharacter* RotateToActor;

	//UPROPERTY()
	//	TArray<class AMastraCTF*> Towers;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "HitReaction")
		TArray<class UBoxComponent*> ActiveColliders;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "HitReaction")
		TArray<class AActor*> ArrDamagedEnemy;

	TEnumAsByte<ETouchIndex::Type> MoveTouchIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<ETouchIndex::Type> RotTouchIndex;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		class UMastraSkillComponent* SkillComp;

	UFUNCTION(NetMulticast, Reliable, WithValidation, Category = "BaseStats")//HP, Damage, Defense
		void SetupBaseStats(float HealthMax, float Def, float MvSpeed, float AttSpeed, float StunDur, float knockBack, float ImmDur);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSetupStats();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void SetupStats();
//
public:

	///*******************Interfaces***********************/
	virtual void Activate_Implementation(); // include a blueprint function
	virtual void ActivatePure(float a, float b, float c, float d, float e, float f, float g) override; //c++ only function

	//virtual void CheckBool_Implementation(bool check);
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
//		void UpdateHUD();
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
//		void CreateCPHUD();

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void RefreshPlayerData();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSetupBaseStats(float HealthMax, float Def, float MvSpeed, float AttSpeed, float StunDur, float knockBack, float ImmDur);
};


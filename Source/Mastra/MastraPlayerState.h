// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MastraInputLibrary.h"
#include "MastraInterface.h"
#include "GameFramework/PlayerState.h"
#include "MastraPlayerState.generated.h"

class USkeletalMesh;
class AMastraCharacter;
/**
 * 
 */
UCLASS()
class MASTRA_API AMastraPlayerState : public APlayerState, public IMastraInterface
{
	GENERATED_BODY()
	
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:

	/////////////Levelling Up and Experience Point///////////////////////
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "EXP")
		int Exp = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "EXP")
		int ExpNeeded = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UnlockedSkill")
		TMap<FString, int32> Skills;
	/////////////////////////////////////////////////////////////////////

public:

	/////////////Team Setup//////////////////
	UPROPERTY(Replicated)
		FString Team;

	UPROPERTY(Replicated)
		FString TeamRef;

	UPROPERTY(Replicated)
		bool bTeamB;

public:

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Money")
		int Honor = 0;

	/////////////Levelling Up///////////////////////
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Level")
		int Level = 0;
	////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float MaxHealth = 450.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float Defense = 750.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float BaseDamagePercent = 0.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float AtkSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float MoveSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float ImmunityDur = 1.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float StunDuration = 1.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		float KnockbackVector = 0.0f;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		FName TeamName;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		FName StyleName;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		FString Username;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		FString AWSPlayerId;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		FString AWSPlayerSessionId;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		FString AWSGameSessionId;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		int32 Pi = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		int32 CurrentStyle = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		int Kill = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		int Death = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		int Assist = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		USkeletalMesh* CharMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Respawn")
		FTransform SpawnTransform;

	//Respawntime
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_InitTimer, BlueprintReadWrite, Category = "Respawn")
		float InitRespawnTime = 0.0f;
	UFUNCTION()
		void OnRep_InitTimer();

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Timer, BlueprintReadWrite, Category = "Item")
		int RespawnTimeCounter = 30;
	UFUNCTION()
		void OnRep_Timer();

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Respawn")
		FTimerHandle RespawnHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* ActionTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UDataTable* LevelTable;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* FrontHitMoveset;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* BackHitMoveset;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* RightHitMoveset;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "HitReaction")
		UAnimMontage* LeftHitMoveset;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Skill")
		class UMastraSkillComponent* SkillComponent;

protected:

	//TMap
	UFUNCTION(BlueprintCallable)
		void AddToMap(FString str);

	UFUNCTION(BlueprintCallable)//checvalcount opt for need to current value from max value needed for a key
		bool CheckKeyInMap(FString str, bool checkValCount, int maxVal);

	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	//	bool IsCurrentlyVisible = false;

public:

	virtual void LookUp_Implementation(const FString& str); //interface
	virtual void ActivatePure(float a, float b, float c, float d, float e, float f, float g) override; //c++ only function

	UFUNCTION(Reliable, Server, WithValidation, Category = "Timer")
		void StartRespawnTimer(AMastraPlayerState* ps);

	UFUNCTION(Reliable, Server, WithValidation, Category = "Timer")
		void RespawnTimerCount(AMastraPlayerState* ps);

	UFUNCTION(Reliable, NetMulticast, WithValidation, Category = "Timer")
		void MulticastTimerCount(int32 val);

	UFUNCTION(Reliable, Client, WithValidation, Category = "PI")
		void SetPlayerIndex(int32 PlayerIndex);

	//For displaying respawn time count
	UFUNCTION(BlueprintImplementableEvent, Category = "Timer")
		void DisplayRespawnTime(int32 val);

	////////////////////Level UP///////////////////////////////////////
	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = "Exp")
		void ServerSetExp(int EXPoint);

	UFUNCTION(Reliable, NetMulticast, WithValidation, Category = "Exp")
		void ClientSetExp(int EXPoint);

	UFUNCTION(BlueprintCallable)
		void AddExp(int EXPoint, int& OutLevel);

	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = "Exp")
		void ServerSetRespawnTime(float time);

	UFUNCTION(Reliable, NetMulticast, WithValidation, Category = "Exp")
		void ClientSetRespawnTime(float time);
	//////////////////////////////////////////////////////////////////

	void SetKDAOnDeath(AMastraPlayerState* victim, AMastraPlayerState* killer, TArray<AMastraPlayerState*> assist);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MastraGameState.generated.h"

/**
 * 
 */
UCLASS()
class MASTRA_API AMastraGameState : public AGameStateBase
{
	GENERATED_BODY()
	
		//Replicated Network setup
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
		FString LatestEvent;

	UPROPERTY(Replicated)
		FString CountdownTimer;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		FString WinningTeam;

	UPROPERTY(Replicated)
		FString PlayerID;

	UPROPERTY(Replicated)
		TArray<FString> ArrayPlayerID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString GameSessionID;

	//--------------------Teams setup-------------------------//
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		TArray <FString> TeamA;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		TArray <FString> TeamB;

	//KDA setup
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		int TeamKillA = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		int TeamKillB = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Kill = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Death = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Assist = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int CurrentKill = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int CurrentDeath = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int CurrentAssist = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int CurrentWin = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int CurrentLose = 0;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		float Timer = 0.0f;

	UPROPERTY(BlueprintReadWrite, Replicated)
		bool UpdateKDACheck = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool UsernameCheck = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool DataCheck = false;

	//------------------Clock--------------------------//
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Clock")
		float CurrentTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Clock")
		FString Winner;// = "Radiant";

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Clock")
		float RemainingGameTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Clock")
		bool LobbyCheck = false;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Clock")
		float LobbyTimer = 0.0f;

	////For displaying respawn time count
	//UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	//	void DisplayRespawnTime();

public:


};

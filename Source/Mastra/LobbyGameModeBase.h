// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "LobbyGameModeBase.generated.h"

class AMastraCharacter;
class AMastraGameState;
class AMastraPlayerState;
/**
 * 
 */
UCLASS()
class MASTRA_API ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		//Replicated Network setup
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ALobbyGameModeBase();

	//virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//virtual void Logout(AController* Exiting) override;

	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Players")
		void UpdateLobby();

	///////////////////////////////////////////inventory/////////////////////////////////////////////
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item System")
	//	FItem FindItem(FName ItemID, bool& IsSuccess);

	//FItem FindItem_Implementation(FName ItemID, bool& IsSuccess);

public:

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "ActorSpawning")
		TArray<class AMastraPlayerController*> Players;

	UPROPERTY()
		FTimerHandle ClockTimer;

	UPROPERTY()
		FTimerHandle LobbyClockTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NewPlayer")
		APlayerController* newPlayer;

protected:

	virtual void BeginPlay() override;

	//virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;

	UFUNCTION()
		void StartClock();

	UFUNCTION()
		void StartLobbyClock();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
		int32 CharIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Game State")
		AMastraGameState* GState;

	UPROPERTY(BlueprintReadWrite, Category = "Game State")
		AMastraPlayerState* PState;

	//************************Clock***********************//
	UPROPERTY(BlueprintReadWrite, Category = "Clock")
		float InitialTimer = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Clock")
		float CurrentTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock")
		float EndTime = 900.0f;


private:

	UPROPERTY()
		float RemainingGameTime;

	UPROPERTY()
		float LobbyTimer;
	
};

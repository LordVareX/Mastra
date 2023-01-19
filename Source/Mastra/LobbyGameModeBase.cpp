// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

//Mastra
#include "MastraCharacter.h"
#include "MastraPlayerState.h"
#include "MastraGameState.h"
#include "MastraPlayerController.h"
#include "MastraInputLibrary.h"
#include "MastraGameInstance.h"

void ALobbyGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameModeBase, Players);
	//DOREPLIFETIME(ALobbyGameModeBase, CharIndex);
}

ALobbyGameModeBase::ALobbyGameModeBase()
{

}

void ALobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	newPlayer = NewPlayer;

	//set mesh array into temp array
	//Chars = CharSelections;

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Player Added : %s"), *newPlayer->GetFName().ToString()));

		//newPlayer = NewPlayer;
		AMastraPlayerController* MobaPC = Cast<AMastraPlayerController>(newPlayer);

		if (MobaPC)
		{
			Players.Add(MobaPC);

			//AMastraPlayerState* PS = Cast <AMastraPlayerState>(MobaPC->PlayerState);
			//if (PS)
			//{
			//	GState = Cast<AMastraGameState>(UGameplayStatics::GetGameState(this));
			//	if (GState)
			//	{
			//		PS->Pi = Players.Num() - 1;
			//		if ((PS->Pi) < 4)
			//		{
			//			GState->TeamA.Add(PS->GetPlayerName());
			//			SpawnBasedOnTeam("Radiant", CharSelections[CharIndex]);
			//			//UpdateLobby();
			//		}
			//		else
			//		{
			//			GState->TeamB.Add(PS->GetPlayerName());
			//			SpawnBasedOnTeam("Dire", CharSelections[CharIndex]);

			//		}
			UpdateLobby();
			//Random unique number for character mesh array
			//if (Chars.Num() > 0)
			//{
			//	CharIndex = FMath::RandRange(0, Chars.Num() - 1);
			//	//PS->CharMesh = Chars[CharIndex];
			//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Player Index : %d"), Players.Num() - 1));
			//	if ((PS->Pi) < 4)
			//	{
			//		GState->TeamA.Add(PS->GetPlayerName());
			//		SpawnBasedOnTeam("Radiant", CharSelections[CharIndex]);
			//	}
			//	else
			//	{
			//		GState->TeamB.Add(PS->GetPlayerName());
			//		SpawnBasedOnTeam("Dire", CharSelections[CharIndex]);
			//	}
			//	Chars.RemoveAtSwap(CharIndex);
			//}
	/*	}
	}*/
	//if (Players.Num() >= 8)
	//{
	//	GetWorldTimerManager().SetTimer(LobbyClockTimer, this, &ALobbyGameModeBase::StartLobbyClock, 1.0f, true);
	//	//UpdateLobby();
	//}
		}
	}
}

void ALobbyGameModeBase::StartLobbyClock() {
	if (GState != nullptr) {
		if (LobbyTimer > 0.0f) {
			LobbyTimer--;
			GState->LobbyTimer = LobbyTimer;
		}
		else {
			GState->LobbyCheck = true;
			GetWorldTimerManager().ClearTimer(LobbyClockTimer);
			UpdateLobby();
			GetWorldTimerManager().SetTimer(ClockTimer, this, &ALobbyGameModeBase::StartClock, 1.0f, true);
		}
	}
}

void ALobbyGameModeBase::StartClock()
{
	if (GState != nullptr)
	{
		/*if (RemainingGameTime < 890.0f) {
			GState->LobbyCheck = true;
		}*/
		//if (CurrentTime < EndTime)
		//{
		//	//Get current timer value
		//	CurrentTime = (UInputLibrary::GetCurrentTimeInMinute()*60.0f) - InitialTimer;
		//	//GState->LatestEvent = FString::FromInt(CurrentTime) + " seconds until the game is over";
		//	GState->CurrentTime = CurrentTime;
		//}

		if (RemainingGameTime > 0.0f) {
			RemainingGameTime--;
			GState->RemainingGameTime = RemainingGameTime;
		}
		else
		{
			GState->LatestEvent = "GameEnded";
			//Stops the timer and check for winners
			GetWorldTimerManager().ClearTimer(ClockTimer);
			if (GState->TeamKillA > GState->TeamKillB)
			{
				GState->Winner = "Radiant";
				//MatchResult("Radiant");
			}
			else if (GState->TeamKillB > GState->TeamKillA)
			{
				GState->Winner = "Dire";
				//MatchResult("Dire");
			}
		}
	}

}
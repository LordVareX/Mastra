// Copyright Epic Games, Inc. All Rights Reserved.

#include "MastraGameMode.h"
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


void AMastraGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMastraGameMode, Players);
	//DOREPLIFETIME(AMastraGameMode, CharIndex);
}

AMastraGameMode::AMastraGameMode()
{
	//LobbyTimer = 10.0f;
	//// use our custom PlayerController class
	//PlayerControllerClass = AMastraPlayerController::StaticClass();

	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	//// set default controller to our Blueprinted controller
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	//if(PlayerControllerBPClass.Class != NULL)
	//{
	//	PlayerControllerClass = PlayerControllerBPClass.Class;
	//}
}

void AMastraGameMode::BeginPlay()
{
	Super::BeginPlay();

	LobbyTimer = 10.0f;

}

//AActor* AMastraGameMode::FIndPlayerStart_Implementation(AController* Player, )
//{
//	AMastraPlayerState* PS = Cast <AMastraPlayerState>(Player->PlayerState);
//	if (PS)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Player"));
//		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
//		{
//			APlayerStart* currentPlayerStart = *It;
//			if (currentPlayerStart->PlayerStartTag == FName(*FString::FromInt(PS->Pi)))
//			{
//				//currentPlayerStart->PlayerStartTag = "Taken";
//				return currentPlayerStart;
//			}
//		}
//	}
//	return nullptr;
//}

void AMastraGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//newPlayer = NewPlayer;

	////set mesh array into temp array
	//Chars = CharSelections;

	//if (HasAuthority())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Player Added : %s"), *newPlayer->GetFName().ToString()));

	//	//newPlayer = NewPlayer;
	//	AMastraPlayerController* MobaPC = Cast<AMastraPlayerController>(newPlayer);

	//	if (MobaPC)
	//	{
	//		Players.Add(MobaPC);

	//		AMastraPlayerState* PS = Cast <AMastraPlayerState>(MobaPC->PlayerState);
	//		if (PS)
	//		{
	//			GState = Cast<AMastraGameState>(UGameplayStatics::GetGameState(this));
	//			if (GState)
	//			{
	//				PS->Pi = Players.Num() - 1;
	//				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Player Index : %d"), Players.Num() - 1));
	//				//for (int i = 0; i < PS->Pi; i = i + 2)
	//				//{
	//					if ((PS->Pi) < 4)
	//					{
	//						GState->TeamA.Add(PS->GetPlayerName());
	//						SpawnBasedOnTeam("Radiant");
	//						//UpdateLobby();
	//					}
	//					else
	//					{
	//						GState->TeamB.Add(PS->GetPlayerName());
	//						SpawnBasedOnTeam("Dire");

	//					}
	//					UpdateLobby();
	//					//Random unique number for character mesh array
	//					//if (Chars.Num() > 0)
	//					//{
	//					//	CharIndex = FMath::RandRange(0, Chars.Num() - 1);
	//					//	//PS->CharMesh = Chars[CharIndex];
	//					//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Player Index : %d"), Players.Num() - 1));
	//					//	if ((PS->Pi) < 4)
	//					//	{
	//					//		GState->TeamA.Add(PS->GetPlayerName());
	//					//		SpawnBasedOnTeam("Radiant", CharSelections[CharIndex]);
	//					//	}
	//					//	else
	//					//	{
	//					//		GState->TeamB.Add(PS->GetPlayerName());
	//					//		SpawnBasedOnTeam("Dire", CharSelections[CharIndex]);
	//					//	}
	//					//	Chars.RemoveAtSwap(CharIndex);
	//					//}
	//				//}
	//			}
	//		}
	//		
	//		if (Players.Num() > 0)
	//		{
	//			GetWorldTimerManager().SetTimer(LobbyClockTimer, this, &AMastraGameMode::StartLobbyClock, 1.0f, true, 1.0f);
	//			//UpdateLobby();
	//		}
	//	}
	//}
}

void AMastraGameMode::SpawnBasedOnTeam(FName TeamName)//(FName TeamName, USkeletalMesh* CharMesh)
{
	AMastraPlayerState* PS = Cast <AMastraPlayerState>(newPlayer->PlayerState);
	if (PS)
	{
		if (HasAuthority())
		{
			FName Team;
			PS->TeamName = TeamName;
			Team = TeamName;
			//PS->CharMesh = CharMesh;

			//AActor* PStart = FindPlayerStart(newPlayer, FString::FromInt(PS->Pi));
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Player Index : %d"), PS->Pi));

			//destroys existing pawn before spawning a new one
			//if (newPlayer->GetPawn() != nullptr)
			//{
			//	newPlayer->GetPawn()->Destroy();
			//}

			//AMastraCharacter* pawn = GetWorld()->SpawnActorDeferred<AMastraCharacter>(SpawnedActor, PStart->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			//if (pawn)
			//{
			//	//pawn->PlayerName = PS->GetPlayerName();
			//	//pawn->TeamName = PS->TeamName;
			//	PS->Team = Team.ToString();
			//	//pawn->CharMesh = PS->CharMesh;
			//	PS->SpawnTransform = PStart->GetActorTransform();
			//	UGameplayStatics::FinishSpawningActor(pawn, FTransform(PStart->GetActorRotation(), PStart->GetActorLocation()));

			//	newPlayer->Possess(pawn);
			//	newPlayer->ClientSetRotation(PStart->GetActorRotation());

			//	/*newPlayer->bShowMouseCursor = true;
			//	newPlayer->SetInputMode(FInputModeGameAndUI());*/
			//}
		}
	}
}

void AMastraGameMode::StartLobbyClock() {
	if (GState != nullptr) {
		if (LobbyTimer > 0.0f) {
			LobbyTimer--;
			GState->LobbyTimer = LobbyTimer;
		}
		else {
			GState->LobbyCheck = true;
			GetWorldTimerManager().ClearTimer(LobbyClockTimer);
			UpdateLobby();
			GetWorldTimerManager().SetTimer(ClockTimer, this, &AMastraGameMode::StartClock, 1.0f, true);
		}
	}
}

void AMastraGameMode::StartClock()
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

		if (RemainingGameTime <= 0.0f) {
			RemainingGameTime++;
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

bool AMastraGameMode::RespawnRequested_Validate(APlayerController* playerController, FTransform SpawnTransform)
{
	return true;
}

void AMastraGameMode::RespawnRequested_Implementation(APlayerController* playerController, FTransform SpawnTransform)
{
	if (playerController != nullptr)
	{
		if (HasAuthority())
		{
			//destroys existing pawn before spawning a new one
			/*if (playerController->GetPawn() != nullptr)
			{
				playerController->GetPawn()->Destroy();
			}*/
			AMastraPlayerState* PS = Cast<AMastraPlayerState>(playerController->PlayerState);
			{
				playerController->ClientSetLocation(SpawnTransform.GetLocation(), SpawnTransform.Rotator());
				//Spawn actor
//				if (SpawnedActor)
//				{
//					//Spawn actor from SpawnedActor subclass
//					AMastraCharacter* pawn = GetWorld()->SpawnActorDeferred<AMastraCharacter>(SpawnedActor, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
//					if (pawn)
//					{
//						//Assign team and player name before finish spawning
//						//pawn->PlayerName = PS->Username;
//						pawn->Team = FCString::Atoi(*PS->TeamName.ToString());
//						//pawn->CharMesh = PS->CharMesh;
//
//						UGameplayStatics::FinishSpawningActor(pawn, FTransform(SpawnTransform.Rotator(), SpawnTransform.GetLocation()));
//					}
//
//					//possess and set new rotation for newly spawned pawn
//					playerController->Possess(pawn);
//					playerController->ClientSetRotation(pawn->GetActorRotation());
//
//					AMastraPlayerController* pc = Cast<AMastraPlayerController>(playerController);
//					if (pc)
//					{
////						pc->SetupPawnAttribute();
//					}
//					/*playerController->bShowMouseCursor = false;
//					playerController->GetPawn()->EnableInput(playerController);*/
//				}
			}
		}
	}
}

bool AMastraGameMode::PopulateShopItem_Validate(AMastraPlayerController* pc)
{
	return true;
}

void AMastraGameMode::PopulateShopItem_Implementation(AMastraPlayerController* pc)
{
	if (pc->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		for (const TPair<FName, FItem>& pair : ItemDatabase->Data)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("Key : %s"), *pair.Key.ToString()));
//			pc->RetrieveArtifactItem(pair.Value);
			/*pair.Key;
			pair.Value;*/
		}
	}
}

void AMastraGameMode::PlayerKilled(AMastraPlayerState* victim, AMastraPlayerState* killer, TArray<AMastraPlayerState*> assist)
{
	FName rowName = RewardTable->GetRowNames()[victim->Level - 1];
	FRewards* row = RewardTable->FindRow<FRewards>(rowName, FString());
	if (row)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("ROW FOUND")));

		if (assist.IsValidIndex(0))
		{
			for (int32 i = 0; i < assist.Num() - 1; i++)
			{
				if (assist[i] != assist.Last())
				{
					assist[i]->Assist += 1;
					GState->Assist = assist[i]->Assist;

					int exp;
					int honor;

					//Get victim level
					if (UMastraInputLibrary::CalculateRewards(HonorKill, assist.Num(), row, rowName, honor, exp))
					{
						//add honor value
						assist[i]->Honor += honor;
						if (assist[i]->GetPawn() != nullptr && assist[i]->GetPawn()->GetClass()->ImplementsInterface(UMastraInterface::StaticClass()))
						{
							//Opens level up windows
							Cast<IMastraInterface>(assist[i]->GetPawn())->Execute_OnActivate(assist[i]->GetPawn(), assist[i]->Username, victim->Username, honor);
						}
						//Set assist experience point
						assist[i]->ClientSetExp(exp);
					}
				}
			}
		}
		if (killer != victim)
		{
			victim->Death += 1;
			GState->Death += 1;
			killer->Kill += 1;
			GState->Kill += 1;

			int exp;
			int honor;

			//Get victim level
			if (UMastraInputLibrary::CalculateRewards(HonorKill, assist.Num(), row, rowName, honor, exp))
			{
				//add honor value
				killer->Honor += honor;
				if (killer->GetPawn() != nullptr && killer->GetPawn()->GetClass()->ImplementsInterface(UMastraInterface::StaticClass()))
				{
					//Opens level up windows
					Cast<IMastraInterface>(killer->GetPawn())->Execute_OnActivate(killer->GetPawn(), killer->Username, victim->Username, honor);
				}
				//Set kill experience point
				killer->ClientSetExp(row->ExpKills);
			}
			if (killer->TeamName == "Radiant")
			{
				GState->TeamKillA += 1;
			}
			else if (killer->TeamName == "Dire")
			{
				GState->TeamKillB += 1;
			}
		}
	}
}
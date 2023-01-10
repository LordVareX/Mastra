// Fill out your copyright notice in the Description page of Project Settings.


#include "MastraGameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MastraCharacter.h"
#include "MastraPlayerState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"

void AMastraGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMastraGameState, LatestEvent);
	DOREPLIFETIME(AMastraGameState, WinningTeam);
	DOREPLIFETIME(AMastraGameState, TeamA);
	DOREPLIFETIME(AMastraGameState, TeamB);
	DOREPLIFETIME(AMastraGameState, TeamKillA);
	DOREPLIFETIME(AMastraGameState, TeamKillB);
	DOREPLIFETIME(AMastraGameState, Timer);
	DOREPLIFETIME(AMastraGameState, CurrentTime);
	DOREPLIFETIME(AMastraGameState, Winner);
	DOREPLIFETIME(AMastraGameState, CountdownTimer);
	DOREPLIFETIME(AMastraGameState, RemainingGameTime);
	DOREPLIFETIME(AMastraGameState, LobbyCheck);
	DOREPLIFETIME(AMastraGameState, LobbyTimer);
	DOREPLIFETIME(AMastraGameState, UpdateKDACheck);
	DOREPLIFETIME(AMastraGameState, PlayerID);
	DOREPLIFETIME(AMastraGameState, ArrayPlayerID);

}

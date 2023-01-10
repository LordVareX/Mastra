// Copyright Epic Games, Inc. All Rights Reserved.

#include "MastraPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MastraCharacter.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Camera/PlayerCameraManager.h"
#include "Blueprint/UserWidget.h"

//Mastra
#include "MastraInputLibrary.h"
#include "MastraGameMode.h"
#include "MastraPlayerState.h"
#include "MastraGameState.h"
#include "MastraCharacter.h"
#include "ItemInterface.h"

AMastraPlayerController::AMastraPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMastraPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMastraPlayerController, pi);
	DOREPLIFETIME(AMastraPlayerController, SpectPI);
}

void AMastraPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMastraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(bInputPressed)
	{
		FollowTime += DeltaTime;

		// Look for the touch location
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		if(bIsTouch)
		{
			GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
		}
		else
		{
			GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		}
		HitLocation = Hit.Location;

		// Direct the Pawn towards that location
		APawn* const MyPawn = GetPawn();
		if(MyPawn)
		{
			FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	}
	else
	{
		FollowTime = 0.f;
	}
}

void AMastraPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMastraPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMastraPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMastraPlayerController::OnTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &AMastraPlayerController::OnTouchReleased);

}

void AMastraPlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AMastraPlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if(FollowTime <= ShortPressThreshold)
	{
		// We look for the location in the world where the player has pressed the input
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AMastraPlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = true;
	OnSetDestinationPressed();
}

void AMastraPlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AMastraPlayerController::AddItem(FName ItemID)
{
	if (EquipmentArtifacts.Contains(ItemID))
	{
		FItem item = *EquipmentArtifacts.Find(ItemID);
		item.Quantity += 1;

		EquipmentArtifacts.Remove(ItemID);
		EquipmentArtifacts.Add(ItemID, item);

		UE_LOG(LogTemp, Warning, TEXT("[Player:AddItem] picked %s - Quantity: %d "), *ItemID.ToString(), item.Quantity);

		OnRefreshInventory();
		return;
	}
	if (EquipmentArtifacts.Num() == TotalEquipmentSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Player:AddItem] Equipment Full: %d/%d "), EquipmentArtifacts.Num() / TotalEquipmentSlots);
		return;
	}

	//Find the item in the table on the game mode to get the information
	/*GM = Cast<AMastraGameMode>(GetWorld()->GetAuthGameMode());
	if (GM != nullptr)
	{
		bool Found = false;
		FItem ItemFound = GM->FindItem(ItemID, Found);

		if (Found)
		{
			FItem NewItem;
			NewItem.ItemID = ItemID;
			NewItem.Name = ItemFound.Name;
			NewItem.Description = ItemFound.Description;
			NewItem.Quantity = 1;
			NewItem.IsActive = ItemFound.IsActive;
			NewItem.ItemIcon = ItemFound.ItemIcon;
			NewItem.ItemEffects = ItemFound.ItemEffects;

			EquipmentArtifacts.Add(NewItem.ItemID, NewItem);
		}
	}*/
	OnRefreshInventory();
}

void AMastraPlayerController::RemoveItem(FName ItemID, bool RemovedFromInventory)
{
	if (EquipmentArtifacts.Contains(ItemID))
	{
		EquipmentArtifacts.Remove(ItemID);
		OnRefreshInventory();
		RemovedFromInventory = true;
		return;
	}
}

bool AMastraPlayerController::HasFreeInventorySlots()
{
	return (EquipmentArtifacts.Num() < TotalEquipmentSlots);
}

FName AMastraPlayerController::GetName()
{
	return FName();
}

void AMastraPlayerController::OnGetValue_Implementation(APlayerController* pc, const FItem& ItemValue)
{

}

void AMastraPlayerController::OnInteract_Implementation(const FName& ItemName)
{

}

bool AMastraPlayerController::SetupPawnAttribute_Validate()
{
	return true;
}

void AMastraPlayerController::SetupPawnAttribute_Implementation()
{
	AMastraCharacter* pawn = Cast<AMastraCharacter>(GetPawn());
	if (pawn)
	{
		/*pawn->MainWidget = MainWidget;*/
		pawn->EnableInput(this);
		pawn->RefreshPlayerData();
	}
}

void AMastraPlayerController::Action()
{
	/*if (Character != nullptr)
	{
		Character->Execute_Item();
	}*/
}

bool AMastraPlayerController::RequestArtifactShops_Validate()
{
	return true;
}

void AMastraPlayerController::RequestArtifactShops_Implementation()
{
	AMastraGameMode* gamemode = Cast<AMastraGameMode>(GetWorld()->GetAuthGameMode());
	if (gamemode)
	{
		gamemode->PopulateShopItem(this);
	}
}

bool AMastraPlayerController::RetrieveArtifactItem_Validate(const FItem& ItemValue)
{
	return true;
}

void AMastraPlayerController::RetrieveArtifactItem_Implementation(const FItem& ItemValue)
{
	if (this->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		Cast<IItemInterface>(this)->Execute_OnGetValue(this, this, ItemValue);//set exist bool in player character to true
	}
}

int32 AMastraPlayerController::CheckIndexValidity(int32 index, TArray<AMastraPlayerController*> PlayerList, EFormula SwitchMode)
{
	if (SwitchMode == EFormula::Addition)
	{
		//select the index number forward
		int32 temp = index + 1;
		if (PlayerList.Num() == temp)
		{
			return 0;
		}
		else
			return temp;
	}
	else
	{
		//select the index number backwards
		int32 temp = index - 1;
		if (temp == -1)
		{
			return PlayerList.Num() - 1;
		}
		else
			return temp;
	}


}

bool AMastraPlayerController::ClientSetInputMode_Validate()
{
	return true;
}

void AMastraPlayerController::ClientSetInputMode_Implementation()
{
	if (this->IsLocalPlayerController() && this->GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		//this->bShowMouseCursor = true;
		this->SetInputMode(FInputModeGameAndUI());
	}
}

bool AMastraPlayerController::SpectateNextPlayer_Validate(const TArray<AMastraPlayerController*>& PlayerList, EFormula SwitchMode)
{
	return true;
}

void AMastraPlayerController::SpectateNextPlayer_Implementation(const TArray<AMastraPlayerController*>& PlayerList, EFormula SwitchMode)
{

	bool x = false;
	int32 count = 0;

	//get playerstate
	AMastraPlayerState* thisps = Cast<AMastraPlayerState>(this->PlayerState);

	for (int32 i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i]->GetPawn() != nullptr)
		{
			AMastraPlayerState* ps = Cast<AMastraPlayerState>(PlayerList[i]->PlayerState);
			if (ps->TeamName == thisps->TeamName)
			{
				count += 1;
				if (count > 0)
				{
					x = true;
					break;
				}
			}
		}
	}

	//Is eligible to spectate??
	if (x == true)
	{
	loop:
		currentPlayer = CheckIndexValidity(currentPlayer, PlayerList, SwitchMode); //check if next spectated player is exist in an array

		if (PlayerList[currentPlayer]->GetPawn() != nullptr) //checks if player does have a pawn
		{
			if (PlayerList[currentPlayer]->GetPawn() != this->GetPawn()) //checks if spectated pawn is not current owning pawn
			{
				AMastraPlayerState* ps = Cast<AMastraPlayerState>(PlayerList[currentPlayer]->PlayerState);
				if (ps->TeamName == thisps->TeamName) //spectating only team pawn
				{
					this->SetViewTargetWithBlend(PlayerList[currentPlayer], 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
					CurrSpectator = PlayerList[currentPlayer];//set new spectated player
					CurrSpectator->SpectPI = this->pi;
					return;
				}
				else
					goto loop;
			}
			else
				goto loop;
		}
		else
			goto loop;
	}
}

bool AMastraPlayerController::SetupSpectator_Validate(EFormula SwitchMode)
{
	return true;
}

void AMastraPlayerController::SetupSpectator_Implementation(EFormula SwitchMode)
{
	if (this->GetPawn() == nullptr) // make sure no owning pawn present before spectating
	{
		GM = Cast<AMastraGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			SpectateNextPlayer(GM->Players, SwitchMode);
		}
	}
}

bool AMastraPlayerController::RespawnPawn_Validate(FTransform SpawnTransform)
{
	return true;
}

void AMastraPlayerController::RespawnPawn_Implementation(FTransform SpawnTransform)
{
	GM = Cast<AMastraGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		//Destroy pawn before respawning
		if (this->GetPawn())
		{
			this->GetPawn()->Destroy();
		}

		FTimerHandle handle;
		FTimerDelegate TimerDelegate;

		//set view target
		TimerDelegate.BindLambda([this]()
		{
			//Assigned initial spectator player before swapping to active pawn player to spectate
			currentPlayer = GM->Players.Find(this);
			CurrSpectator = GM->Players[currentPlayer];
			CurrSpectator->SpectPI = this->pi;

			this->SpectateNextPlayer(GM->Players, EFormula::Addition);

			//Setup spectator controller that currently spectating this player to switch to another player
			AMastraPlayerController* newPC = Cast<AMastraPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), CurrSpectator->SpectPI));
			if (newPC)
			{
				if (newPC->GetPawn() == nullptr)
				{
					newPC->currentPlayer = GM->Players.Find(newPC);
					newPC->CurrSpectator = GM->Players[currentPlayer];
					newPC->CurrSpectator->SpectPI = newPC->pi;

					newPC->SpectateNextPlayer(GM->Players, EFormula::Addition);
				}
			}

		});
		this->GetWorldTimerManager().SetTimer(handle, TimerDelegate, 0.02f, false);

		//get current controller playerstate
		AMastraPlayerState* thisstate = Cast<AMastraPlayerState>(this->PlayerState);

		float temp = thisstate->InitRespawnTime - 3.0f;

		//Delay before respawning a new pawn
		FTimerHandle handle1;
		FTimerDelegate TimerDelegate1;

		//Possess a pawn
		TimerDelegate1.BindLambda([this, thisstate]()
		{
			PlayerCameraManager->BlendTimeToGo = 0.0f;
			GM->RespawnRequested(this, thisstate->SpawnTransform);
		});
		this->GetWorldTimerManager().SetTimer(handle1, TimerDelegate1, temp, false);
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "MastraInputLibrary.h"
#include "ItemInterface.h"
#include "MastraPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class AMastraGameMode;

UCLASS()
class AMastraPlayerController : public APlayerController, public IItemInterface
{
	GENERATED_BODY()

	//Replicated Network setup
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	AMastraPlayerController();

	virtual void BeginPlay() override;

	///////////////////////////////Artifacts Interaction//////////////////////////////////// 

	void AddItem(FName ItemID);

	void RemoveItem(FName ItemID, bool RemovedFromInventory);

	bool HasFreeInventorySlots();

	//interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void OnInteract(const FName& ItemName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
		void OnGetValue(APlayerController* pc, const FItem& ItemValue);

	//Actual implementation of the Interact event
	void OnGetValue_Implementation(APlayerController* pc, const FItem& ItemValue);
	void OnInteract_Implementation(const FName& ItemName);
	virtual FName GetName() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
		void OnRefreshInventory();

	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
		void RequestArtifactShops();

	UFUNCTION(BlueprintCallable, Reliable, Client, WithValidation)
		void RetrieveArtifactItem(const FItem& ItemValue);

	//////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY()
		AMastraGameMode* GM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spectator")
		int32 currentPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spectator")
		AMastraPlayerController* CurrSpectator;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "ID")
		int32 pi;

	//RequestRespawn
	UFUNCTION(Reliable, Server, WithValidation, Category = "Respawn")
		void RespawnPawn(FTransform SpawnTransform);

	UFUNCTION(BlueprintImplementableEvent, Category = "Players")
		void NameChangeRequest(const FString& PlayerName, int32 PIndex);

	UFUNCTION(Reliable, Client, WithValidation, Category = "Respawn")
		void SetupPawnAttribute();

	//Widget holder
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD", Meta = (ExposeOnSpawn = "true"))
		//UUserWidget* MainWidget = nullptr;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:

	//Equipment artifacts on UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artifacts")
		TMap<FName, FItem> EquipmentArtifacts;

	//Max slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artifacts")
		int32 TotalEquipmentSlots = 4;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

	UFUNCTION()
		void Action();

	//spectator pi
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "SpectID")
		int32 SpectPI;

	UFUNCTION(BlueprintPure, Category = "Increment")
		static int32 CheckIndexValidity(int32 index, TArray<AMastraPlayerController*> PlayerList, EFormula SwitchMode);

	UFUNCTION(BlueprintCallable, Reliable, Client, WithValidation, Category = "InputMode")
		void ClientSetInputMode();

	//RequestSpectator
	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = "Spectator")
		void SetupSpectator(EFormula SwitchMode);

	//SpectatorMode
	UFUNCTION(Reliable, Server, WithValidation, Category = "Spectator")
		void SpectateNextPlayer(const TArray<AMastraPlayerController*>& PlayerList, EFormula SwitchMode);

private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};



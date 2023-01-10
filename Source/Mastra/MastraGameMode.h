
#pragma once

#include "CoreMinimal.h"
#include "MastraInputLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "MastraGameMode.generated.h"

class AMastraCharacter;
class AMastraGameState;
class AMastraPlayerState;

UCLASS(minimalapi)
class AMastraGameMode : public AGameModeBase
{
	GENERATED_BODY()

	//Replicated Network setup
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AMastraGameMode();

	//virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//virtual void Logout(AController* Exiting) override;

	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(Reliable, Server, WithValidation, Category = "ArtifactShop")
		void PopulateShopItem(AMastraPlayerController* pc);

	UFUNCTION(Reliable, Server, WithValidation, Category = "Respawn")
		void RespawnRequested(APlayerController* playerController, FTransform SpawnTransform);

	UFUNCTION(BlueprintCallable, Category = "Score")
		void PlayerKilled(AMastraPlayerState* victim, AMastraPlayerState* killer, TArray<AMastraPlayerState*> assist);

	////Get row from Rewards datatable
	//FRewards* GetRewardsData(FName& rowName, int rowIndex);

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

	UFUNCTION(BlueprintCallable)
		void SpawnBasedOnTeam(FName TeamName, USkeletalMesh* CharMesh);

	TArray<USkeletalMesh*> Chars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TArray<USkeletalMesh*> CharSelections;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
		int32 CharIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Game State")
		AMastraGameState* GState;

	UPROPERTY(BlueprintReadWrite, Category = "Game State")
		AMastraPlayerState* PState;

	/*Blueprint Reference of ThirdPersonCharacter class*/
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AMastraCharacter> SpawnedActor;

	//************************Clock***********************//
	UPROPERTY(BlueprintReadWrite, Category = "Clock")
		float InitialTimer = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Clock")
		float CurrentTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clock")
		float EndTime = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		FString MapName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UDataTable* RewardTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item System")
		class UItemData* ItemDatabase;

private:

	//Initial honor val per kill
	const int HonorKill = 200;

	UPROPERTY()
		float RemainingGameTime;

	UPROPERTY()
		float LobbyTimer;

};




// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h" 
#include "MastraInputLibrary.h"
#include "GameFramework/Character.h"
#include "MastraAnimInstance.h"
#include "MastraInterface.h"
#include "MastraCharacter.generated.h"

class UMastraGameInstance;

UCLASS(config = Game)
class AMastraCharacter : public ACharacter
{
	GENERATED_BODY()

		//Replicated Network setup
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	AMastraCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Status")
		int Team;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		FString PickedCharacter;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		FName PlayerName;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		int Level = 1;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_HealthUpdated, BlueprintReadWrite, Category = "Status")
		float Health;

	UFUNCTION()
		void OnRep_HealthUpdated();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateUI(float currentHealth);//, float currentMaxHealth, float currentLevel);

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MaxHP;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float HPRegen;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float Mana;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MaxMana;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float ManaRegen;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float PhysicalAttack;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MagicPower;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float PhysicalDefense;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MagicDefense;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float PhysicalPenetration;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MagicalPenetration;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float DamageReduction;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float AttackSpeedRatio;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float RespawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* CharacterTable;

	UPROPERTY(BlueprintReadWrite, Category = "Game Instance")
		UMastraGameInstance* GI;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite)
		FTransform SpawnTransform;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite)
		USkeletalMesh* CMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UObject> CAnimBP;
	

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
		void FinishSetupBeginPlay();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSetupStats(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void SetupStats(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSetupDetails(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void SetupDetails(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerLocked(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void Locked(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerLevelIncreased(const FString& CharacterName);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void LevelIncreased(const FString& CharacterName);

	UFUNCTION()
		void RegenHP();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void RespawnCharacter();

	UFUNCTION(BlueprintImplementableEvent)
		void RespawnUpdate();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStats(float currMaxHealth, int32 currLevel);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void RefreshPlayerData();

	UPROPERTY()
		FTimerHandle HPRegenHandle;

	UPROPERTY()
		FTimerHandle RespawnHandle;



};


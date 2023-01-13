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

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		int Team;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		FName PlayerName;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		int Level = 1;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float Health;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category = "Status")
		float HPRegen;

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
		void ServerSetupStats();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void SetupStats();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSetupDetails();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		void SetupDetails();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	



public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void RefreshPlayerData();

	UPROPERTY()
		FTimerHandle HPRegenHandle;

	UFUNCTION()
		void RegenHP();

	UFUNCTION(BlueprintCallable)
		void LevelIncreased();

};


// Fill out your copyright notice in the Description page of Project Settings.


#include "MastraPlayerState.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "MastraInputLibrary.h"
#include "MastraCharacter.h"
#include "MastraInterface.h"
#include "MastraGameState.h"
#include "MastraGameMode.h"

void AMastraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMastraPlayerState, Level);
	DOREPLIFETIME(AMastraPlayerState, Honor);
	DOREPLIFETIME(AMastraPlayerState, ExpNeeded);
	DOREPLIFETIME(AMastraPlayerState, Exp);
	DOREPLIFETIME(AMastraPlayerState, Pi);
	DOREPLIFETIME(AMastraPlayerState, CurrentStyle);
	DOREPLIFETIME(AMastraPlayerState, Kill);
	DOREPLIFETIME(AMastraPlayerState, Death);
	DOREPLIFETIME(AMastraPlayerState, Assist);
	DOREPLIFETIME(AMastraPlayerState, Username);
	DOREPLIFETIME(AMastraPlayerState, AWSPlayerId);
	DOREPLIFETIME(AMastraPlayerState, AWSPlayerSessionId);
	DOREPLIFETIME(AMastraPlayerState, AWSGameSessionId);
	DOREPLIFETIME(AMastraPlayerState, TeamName);
	DOREPLIFETIME(AMastraPlayerState, Team);
	DOREPLIFETIME(AMastraPlayerState, CharMesh);
	DOREPLIFETIME(AMastraPlayerState, bTeamB);
	DOREPLIFETIME(AMastraPlayerState, TeamRef);
	DOREPLIFETIME(AMastraPlayerState, RespawnTimeCounter);
	DOREPLIFETIME(AMastraPlayerState, InitRespawnTime);
	DOREPLIFETIME(AMastraPlayerState, RespawnHandle);
	DOREPLIFETIME(AMastraPlayerState, MaxHealth);
	DOREPLIFETIME(AMastraPlayerState, Defense);
	DOREPLIFETIME(AMastraPlayerState, BaseDamagePercent);
	DOREPLIFETIME(AMastraPlayerState, StyleName);
	DOREPLIFETIME(AMastraPlayerState, FrontHitMoveset);
	DOREPLIFETIME(AMastraPlayerState, BackHitMoveset);
	DOREPLIFETIME(AMastraPlayerState, RightHitMoveset);
	DOREPLIFETIME(AMastraPlayerState, LeftHitMoveset);
	DOREPLIFETIME(AMastraPlayerState, SkillComponent);
	DOREPLIFETIME(AMastraPlayerState, AtkSpeed);
	DOREPLIFETIME(AMastraPlayerState, MoveSpeed);
	DOREPLIFETIME(AMastraPlayerState, ImmunityDur);
	DOREPLIFETIME(AMastraPlayerState, StunDuration);
	DOREPLIFETIME(AMastraPlayerState, KnockbackVector);

}

void AMastraPlayerState::OnRep_InitTimer()
{

}

void AMastraPlayerState::OnRep_Timer()
{
	this->DisplayRespawnTime(RespawnTimeCounter);
}

bool AMastraPlayerState::MulticastTimerCount_Validate(int32 val)
{
	return true;
}

void AMastraPlayerState::MulticastTimerCount_Implementation(int32 val)
{
	RespawnTimeCounter = val;
	OnRep_Timer();
}

bool AMastraPlayerState::SetPlayerIndex_Validate(int32 PlayerIndex)
{
	return true;
}

void AMastraPlayerState::SetPlayerIndex_Implementation(int32 PlayerIndex)
{
	this->Pi = PlayerIndex;
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("PlayerIndex : %f"), this->Pi));
}

bool AMastraPlayerState::StartRespawnTimer_Validate(AMastraPlayerState* ps)
{
	return true;
}

void AMastraPlayerState::StartRespawnTimer_Implementation(AMastraPlayerState* ps)
{
	FTimerDelegate FunctionsName;

	//FunctionsName = FTimerDelegate::CreateUObject(this, &ATodakBattleArenaCharacter::UpdateHealthStatusBar, EBarType::PrimaryProgressBar);
	FunctionsName = FTimerDelegate::CreateUObject(this, &AMastraPlayerState::RespawnTimerCount, ps);

	UE_LOG(LogTemp, Warning, TEXT("RespawnTimer started!"));
	GetWorld()->GetTimerManager().SetTimer(RespawnHandle, FunctionsName, 1.0f, true);
}

bool AMastraPlayerState::RespawnTimerCount_Validate(AMastraPlayerState* ps)
{
	return true;
}

void AMastraPlayerState::RespawnTimerCount_Implementation(AMastraPlayerState* ps)
{
	if (ps->RespawnTimeCounter <= 0)
	{
		this->GetWorldTimerManager().ClearTimer(RespawnHandle);
		ps->RespawnTimeCounter = (int)ps->InitRespawnTime;
		MulticastTimerCount(ps->RespawnTimeCounter);
	}
	else if (ps->RespawnTimeCounter > 0)
	{
		ps->RespawnTimeCounter -= 1;
		MulticastTimerCount(ps->RespawnTimeCounter);
	}
}

bool AMastraPlayerState::CheckKeyInMap(FString str, bool checkValCount, int maxVal)
{
	return UMastraInputLibrary::CheckKeyInTMap(&Skills, str, checkValCount, maxVal);
}

void AMastraPlayerState::AddToMap(FString str)
{
	Skills = *UMastraInputLibrary::AddToTMap(&Skills, str);
}

void AMastraPlayerState::LookUp_Implementation(const FString& str)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("%s"), *str));
	if (Skills.Contains(str))//if the skill is already unlocked and stored in the Skill TMap
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("TRUE")));
		if (GetPawn()->GetClass()->ImplementsInterface(UMastraInterface::StaticClass()))
		{
			Cast<IMastraInterface>(GetPawn())->Execute_CheckBool(GetPawn(), Skills.Contains(str));//set exist bool in player character to true
		}
	}
}

void AMastraPlayerState::ActivatePure(float a, float b, float c, float d, float e, float f, float g)
{
}

bool AMastraPlayerState::ServerSetExp_Validate(int EXPoint)
{
	return true;
}

void AMastraPlayerState::ServerSetExp_Implementation(int EXPoint)
{
	ClientSetExp(EXPoint);
}

bool AMastraPlayerState::ClientSetExp_Validate(int EXPoint)
{
	return true;
}

void AMastraPlayerState::ClientSetExp_Implementation(int EXPoint)
{
	AddExp(EXPoint, this->Level);
}

void AMastraPlayerState::AddExp(int EXPoint, int& OutLevel)
{
	//Get last row of datatable
	FName lastRowName = LevelTable->GetRowNames()[LevelTable->GetRowNames().Num() - 1];
	FCharacterAttributes* lastRow = LevelTable->FindRow<FCharacterAttributes>(lastRowName, FString());

	Exp = FMath::Clamp(Exp + EXPoint, 0, lastRow->MinExpPerLevel);

	//if enough exp, level up
	if (Exp >= ExpNeeded)
	{
		//Level++;

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Exp : %d"), EXPoint));

		Level = FMath::Clamp(Level + 1, 1, lastRow->Level);
		/*Level++;*/

		//Set the remainder exp from prev level to current exp of current level
		//Exp = Exp - ExpNeeded;

		//Get new row of datatable
		if (LevelTable->GetRowNames().IsValidIndex(Level))
		{
			FName newRowName = LevelTable->GetRowNames()[Level];
			FCharacterAttributes* newRow = LevelTable->FindRow<FCharacterAttributes>(newRowName, FString());

			if (newRow)
			{
				//to clamp exp value needed for each level
				ExpNeeded = newRow->MinExpPerLevel;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("ExpNeeded : %d"), ExpNeeded));

				//Adjust base stats
				FName curRowName = LevelTable->GetRowNames()[Level - 1];
				FCharacterAttributes* Row = LevelTable->FindRow<FCharacterAttributes>(curRowName, FString());
				if (Row)
				{
					MaxHealth = UMastraInputLibrary::ChangeValueByPercentage(MaxHealth, Row->HP, true);
					Defense = UMastraInputLibrary::ChangeValueByPercentage(Defense, Row->Physical_Defense, true);
					BaseDamagePercent = Row->Physical_Attack;
					if (GetPawn()->IsLocallyControlled())
					{
						ServerSetRespawnTime(Row->Respawn_Time);
					}
					/*RespawnTimeCounter = Row->RespawnTime;
					OnRep_Timer();

					InitRespawnTime = RespawnTimeCounter;
					OnRep_InitTimer();*/

					/*Row->SkillUnlock;*/

					if (GetPawn()->GetClass()->ImplementsInterface(UMastraInterface::StaticClass()))
					{
						Cast<IMastraInterface>(GetPawn())->ActivatePure(MaxHealth, Defense, MoveSpeed, AtkSpeed, StunDuration, KnockbackVector, ImmunityDur);
					}
				}
			}
		}

		//return level
		OutLevel = Level;
	}
}

bool AMastraPlayerState::ClientSetRespawnTime_Validate(float time)
{
	return true;
}

void AMastraPlayerState::ClientSetRespawnTime_Implementation(float time)
{
	RespawnTimeCounter = time;
	OnRep_Timer();

	InitRespawnTime = RespawnTimeCounter;
	OnRep_InitTimer();
}

bool AMastraPlayerState::ServerSetRespawnTime_Validate(float time)
{
	return true;
}

void AMastraPlayerState::ServerSetRespawnTime_Implementation(float time)
{
	ClientSetRespawnTime(time);
}
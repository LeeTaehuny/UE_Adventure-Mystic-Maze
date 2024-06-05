// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData/MMCharacterStat.h"
#include "MMGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMMGameInstance();

public:
	FORCEINLINE FMMCharacterStat GetPlayerStat(int32 InLevel) const
	{
		return PlayerStatTable.IsValidIndex(InLevel - 1) ? PlayerStatTable[InLevel - 1] : FMMCharacterStat();
	}
	FORCEINLINE int32 GetMaxLevel() { return MaxLevel; }
	FORCEINLINE FString GetSlotName() { return SaveSlotName; }
	FORCEINLINE class UMMSaveGameData* GetSaveData() { return SaveDataInstance; }

	void Exit();
	void SetSaveSlot(int32 InIndex);

private:
	TArray<FMMCharacterStat> PlayerStatTable;
	int32 MaxLevel;
	FString SaveSlotName;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMMSaveInitData> InItData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMMSaveGameData> SaveDataInstance;
};

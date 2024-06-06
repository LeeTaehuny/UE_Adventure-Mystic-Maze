// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameData/MMCharacterStat.h"
#include "MM_Dungeon_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMM_Dungeon_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMM_Dungeon_GameMode();

public:
	FORCEINLINE FMMCharacterStat GetPlayerStat(int32 InLevel) const
	{
		return PlayerStatTable.IsValidIndex(InLevel - 1) ? PlayerStatTable[InLevel - 1] : FMMCharacterStat();
	}

private:
	TArray<FMMCharacterStat> PlayerStatTable;
	int32 MaxLevel;

};

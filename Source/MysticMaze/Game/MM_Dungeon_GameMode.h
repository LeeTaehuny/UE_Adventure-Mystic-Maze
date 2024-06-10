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

	virtual void PreInitializeComponents() override;

public:
	FORCEINLINE FMMCharacterStat GetPlayerStat(int32 InLevel) const
	{
		return PlayerStatTable.IsValidIndex(InLevel - 1) ? PlayerStatTable[InLevel - 1] : FMMCharacterStat();
	}

	void SetRoomCount(FVector INData);
	FORCEINLINE int32 GetRoomCount() { return RoomCount; }

private:
	TArray<FMMCharacterStat> PlayerStatTable;
	int32 MaxLevel;

protected:

	virtual void BeginPlay() override;

	TSubclassOf<class AMMRoomBase> FirstRoomData;
	TSubclassOf<class AMMRoomBase> SecondRoomData;
	TSubclassOf<class AMMRoomBase> ThirdRoomData;

	TSubclassOf<class UMM_Dungeon_UI> DungeongUIOrigin;
	TObjectPtr<class UMM_Dungeon_UI> DungeongUI;

	TSubclassOf<class AMMPortal> PortalOrigin;


	uint8 SpawnStartRoom : 2;
	int32 RoomCount;

};

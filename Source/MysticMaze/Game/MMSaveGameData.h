// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameData/MMCharacterStat.h"
#include "GameData/MMEnums.h"
#include "GameData/MMStructures.h"
#include "MMSaveGameData.generated.h"


/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Save game data file name
	UPROPERTY()
	FString SaveSlotName;

	// Save game data file index
	UPROPERTY()
	int32 SaveIndex;

// Inventory
public:
	// 현재 골드량
	UPROPERTY()
	int32 Gold;

	// 인벤토리 내부 내용물 (인덱스, 이름, 수량)
	UPROPERTY()
	TArray<FMMItemSaveData> InventoryEquipmentArray;
	UPROPERTY()
	TArray<FMMItemSaveData> InventoryConstableArray;
	UPROPERTY()
	TArray<FMMItemSaveData> InventoryOtherArray;

	// 장착중인 무기 이름
	UPROPERTY()
	FName EquipmentWeapon;

	// 퀵슬롯에 등록된 포션 인덱스
	UPROPERTY()
	TArray<int32> PotionSlotIndexs;

// Status
public:
	// 레벨
	UPROPERTY()
	int32 Level;

	// 현재 경험치
	UPROPERTY()
	float CurrentExp;

	// 잔여 스탯 포인트
	UPROPERTY()
	int32 StatPoint;

	// 현재 분배된 스탯 정보
	UPROPERTY()
	FMMCharacterStat ModifierStatus;

	// 클래스 정보
	UPROPERTY()
	EClassType Class;

// SKill
public:
	// 스킬 정보 (이름, 레벨, 퀵슬롯 등록 정보)
	UPROPERTY()
	TArray<FMMSkillSaveData> SkillData;

// Dungeon
public:
	UPROPERTY()
	int32 CurrentFloor;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameData/MMEnums.h"
#include "MMSaveInitData.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSaveInitData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 시작 골드량
	UPROPERTY(EditAnywhere, Category = "StartData")
	int32 Gold;

	// 시작 레벨
	UPROPERTY(EditAnywhere, Category = "StartData")
	int32 Level;

	// 시작 직업
	UPROPERTY(EditAnywhere, Category = "StartData")
	EClassType Class;

	// 클리어 층 수
	UPROPERTY(EditAnywhere, Category = "StartData")
	int32 CurrentFloor;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameData/MMEnums.h"
#include "MMSkillData.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkillData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MMSkillData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Skill)
	EClassType ClassType;

	UPROPERTY(EditAnywhere, Category = Skill)
	FString SkillName;

	UPROPERTY(EditAnywhere, Category = Skill)
	FString SkillDescription;

	UPROPERTY(EditAnywhere, Category = Skill)
	TObjectPtr<class UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere, Category = Skill)
	float CoolTime;

	UPROPERTY(EditAnywhere, Category = Skill)
	float ManaCost;

	UPROPERTY(EditAnywhere, Category = Skill)
	TArray<float> Multiplier;

	UPROPERTY(EditAnywhere, Category = Skill)
	TArray<int32> UpgradeGold; 

	UPROPERTY(EditAnywhere, Category = Skill)
	TSubclassOf<class UMMSkillBase> SkillClass;
};

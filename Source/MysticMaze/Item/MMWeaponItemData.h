// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MMItemData.h"
#include "GameData/MMCharacterStat.h"
#include "GameData/MMEnums.h"
#include "MMWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMWeaponItemData : public UMMItemData
{
	GENERATED_BODY()
	
public:
	UMMWeaponItemData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MMItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AMMWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 ItemMaterialQuantity;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FMMCharacterStat WeaponStat;

	UPROPERTY(EditAnywhere, Category = Weapon)
	EClassType PurchaseableClass;
};

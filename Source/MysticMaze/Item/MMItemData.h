// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MMItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 
{ 
	IT_Weapon,		// 무기
	IT_Potion,		// 포션
	IT_ManaStone,	// 마석
};


/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Base)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Base)
	TObjectPtr<UTexture2D> ItemTexture;

	UPROPERTY(EditAnywhere, Category = Base)
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = Base)
	int32 ItemPurchasePrice;

	UPROPERTY(EditAnywhere, Category = Base)
	int32 ItemSalePrice;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MMItemData.h"
#include "MMPotionItemData.generated.h"

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	PT_Hp,		// 체력 포션
	PT_Mp,		// 마나 포션
};

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMPotionItemData : public UMMItemData
{
	GENERATED_BODY()
	
public:
	UMMPotionItemData();
	
public:
	UPROPERTY(EditAnywhere, Category = Potion)
	EPotionType PotionType;

	UPROPERTY(EditAnywhere, Category = Potion)
	float Percent;
};

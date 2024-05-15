// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ToolTip/MMToolTip.h"
#include "MMEquipmentToolTip.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMEquipmentToolTip : public UMMToolTip
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_ItemType;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_WeaponType;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_STR;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_DEX;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CON;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_INT;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_PurchasePrice;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_SalePrice;
};

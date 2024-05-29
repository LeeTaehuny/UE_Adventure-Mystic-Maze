// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MMInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	UMMInventoryItem();

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMMItemData> ItemData;

	UPROPERTY(VisibleAnywhere)
	int32 ItemQuantity;
};

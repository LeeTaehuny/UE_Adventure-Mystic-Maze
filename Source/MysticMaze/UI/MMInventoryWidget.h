// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMInventoryWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void UpdateInventorySlot();
	void UpdateInventoryGold();

private:
	UFUNCTION()
	void SetEquipmentType();

	UFUNCTION()
	void SetConsumableType();

	UFUNCTION()
	void SetOtherType();

// Main
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_MainButton;

// Header
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Equipment;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Consumable;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Other;

// Tail
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_SortItem;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_Gold;

private:
	void SetType(ESlotType Type);

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UMMSlot>> Slots;

	ESlotType InventorySlotType;
	
};

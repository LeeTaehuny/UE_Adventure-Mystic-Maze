// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMShopWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
public:
	void Init(AActor* InPlayer);
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateInventorySlot();
	void UpdateGold();
	void InitShopSlot();

public:
	UFUNCTION()
	void SetEquipmentType();

	UFUNCTION()
	void SetConsumableType();

	UFUNCTION()
	void SetOtherType();

	UFUNCTION()
	void Close();

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Close;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Equipment;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Consumable;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Other;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CurrentGold;

private:
	UPROPERTY(VisibleAnywhere, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> NPC;

	UPROPERTY(VisibleAnywhere, Category = "Shop")
	TArray<TObjectPtr<class UMMSlot>> ShopSlots;

	UPROPERTY(VisibleAnywhere, Category = "Shop")
	TArray<TObjectPtr<class UTextBlock>> ShopItemNames;

	UPROPERTY(VisibleAnywhere, Category = "Shop")
	TArray<TObjectPtr<class UTextBlock>> ShopItemAmounts;

	UPROPERTY(VisibleAnywhere, Category = "Shop")
	TArray<TObjectPtr<class UMMCustomButton>> ShopPurchaseButtons;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UMMSlot>> InvSlots;

	// 플레이어 인벤토리 슬롯 타입
	ESlotType InventorySlotType;

	// 플레이어 인벤토리 슬롯 변경 함수
	void SetType(ESlotType Type);

	// 플레이어 저장용 변수
	UPROPERTY()
	TObjectPtr<AActor> TempPlayer;
};

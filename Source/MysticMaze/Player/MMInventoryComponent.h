// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MMEnums.h"
#include "MMInventoryComponent.generated.h"

// 인벤토리 내용 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnChangedInventoryDelegate);
// 골드 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnChangedGoldDelegate);
// 퀵슬롯(포션) 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnChangedPotionSlotDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTICMAZE_API UMMInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMMInventoryComponent();

// Delegate
public:
	FOnChangedInventoryDelegate OnChangeInven;
	FOnChangedGoldDelegate OnChangeGold;
	FOnChangedPotionSlotDelegate OnChangedPotionSlot;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	// Getter
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetEquipmentItems() { return EquipmentItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetConsumableItems() { return ConsumableItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetOtherItems() { return OtherItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>>& GetPotionQuickSlots() { return PotionQuickSlots; }
	FORCEINLINE TObjectPtr<class UMMInventoryItem> GetEquipmentItem() { return EquipmentItem; }
	FORCEINLINE int32 GetCurrentGold() { return CurrentGold; }

	// 아이템 추가
	bool AddItem(FName InItemName, int32 InItemQuantity, int32& OutItemQuantity);
	// 아이템 사용
	void UseItem(int32 InSlotIndex, ESlotType InventoryType);
	// 골드 추가
	void AddGold(int32 InGold);
	// 슬롯 교체
	void SwapItem(int32 InPrevIndex, int32 InCurrentIndex, ESlotType InPrevSlotType, ESlotType InCurrentSlotType);
	// 슬롯 정렬
	void SortItem(ESlotType InSlotType);

	// 퀵슬롯 등록
	void SetQuickSlot(ESlotType InPrevSlotType, int32 InPrevIndex, int32 InCurrentIndex);
	// 장비 착용 & 해제
	void EquipItem(int32 InCurrentIndex);
	void UnEquipItem();

// Inventory Section
protected:
	// 인벤토리 초기화
	void InitInventory();
	// 아이템 삭제
	void RemoveItem(int32 InSlotIndex, ESlotType InventoryType);

	// 장비
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> EquipmentItems;

	// 소비
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> ConsumableItems;

	// 기타
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> OtherItems;

	// 인벤토리 슬롯의 수
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxInventoryNum;

	// 한 칸에 저장 가능한 아이템 수
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxItemNum;

	// 현재 보유한 골드
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 CurrentGold;

// QuickSlot Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "QuickSlot", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> PotionQuickSlots;

// Equipment Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "EquipmentSlot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMMInventoryItem> EquipmentItem;
};

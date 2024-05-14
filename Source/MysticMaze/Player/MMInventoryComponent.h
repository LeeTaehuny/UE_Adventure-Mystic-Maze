// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MMEnums.h"
#include "MMInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangedInventoryDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChangedGoldDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTICMAZE_API UMMInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMMInventoryComponent();

	virtual void InitializeComponent() override;

// Delegate
public:
	FOnChangedInventoryDelegate OnChangeInven;
	FOnChangedGoldDelegate OnChangeGold;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetEquipmentItems() { return EquipmentItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetConsumableItems() { return ConsumableItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetOtherItems() { return OtherItems; }
	FORCEINLINE int32 GetCurrentGold() { return CurrentGold; }

	bool AddItem(FName InItemName, int32 InItemQuantity, int32& OutItemQuantity);
	void UseItem(int32 InSlotIndex, ESlotType InventoryType);
	void AddGold(int32 InGold);
	void SwapItem(int32 InPrevIndex, int32 InCurrentIndex, ESlotType InPrevSlotType, ESlotType InCurrentSlotType);
	void SortItem(ESlotType InSlotType);

protected:
	void InitInventory();
	void RemoveItem(int32 InSlotIndex, ESlotType InventoryType);

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> EquipmentItems;

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> ConsumableItems;

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMInventoryItem>> OtherItems;

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxInventoryNum;

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxItemNum;

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 CurrentGold;
};

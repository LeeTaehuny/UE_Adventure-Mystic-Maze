// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMSlot.generated.h"

DECLARE_DELEGATE(FOnUpdateSlotDelegate);
USTRUCT(BlueprintType)
struct FUpdateSlotDelegateWrapper
{
	GENERATED_BODY()
	
	FUpdateSlotDelegateWrapper() { }
	FUpdateSlotDelegateWrapper(const FOnUpdateSlotDelegate& InSlotDelegate) : SlotDelegate(InSlotDelegate) { }

	FOnUpdateSlotDelegate SlotDelegate;
};

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSlot : public UMMCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void Init();
	void SetType(ESlotType Type);
	void UpdateSlot();

public:
	UPROPERTY(VisibleAnywhere, Category = "Slot")
	ESlotType SlotType;

	UPROPERTY(VisibleAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	TObjectPtr<class UImage> IMG_Item;

	UPROPERTY(VisibleAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_Quantity;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;

protected:
	UPROPERTY()
	TMap<ESlotType, FUpdateSlotDelegateWrapper> SlotUpdateActions;

	void UpdateEquipmentSlot();
	void UpdateConsumableSlot();
	void UpdateOtherSlot();

	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTexture2D> DefaultTexture;
};

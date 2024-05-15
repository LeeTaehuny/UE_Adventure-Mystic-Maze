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

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	
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

	// Drag에 사용될 WidgetClass
	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UMMSlot> DragWidgetClass;

// ToolTips
public:
	UPROPERTY(EditAnywhere, Category = "Slot")
	TMap<ESlotType, TSubclassOf<class UMMToolTip>> ToolTipClassMap;

	UPROPERTY(VisibleAnywhere, Category = "Slot")
	TMap<ESlotType, TObjectPtr<class UMMToolTip>> ToolTipMaps;

protected:
	UPROPERTY()
	TMap<ESlotType, FUpdateSlotDelegateWrapper> SlotUpdateActions;

	void UpdateEquipmentSlot();
	void UpdateConsumableSlot();
	void UpdateOtherSlot();
	
	void SetEquipmentToolTip(class UMMToolTip* EquipmentToolTipWidget, class UMMItemData* ItemData);
	void SetConsumableToolTip(class UMMToolTip* ConsumableToolTipWidget, class UMMItemData* ItemData);
	void SetOtherToolTip(class UMMToolTip* OtherToolTipWidget, class UMMItemData* ItemData);

	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTexture2D> DefaultTexture;
};

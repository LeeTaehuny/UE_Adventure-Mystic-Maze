// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMInventoryWidget.h"
#include "Interface/MMInventoryInterface.h"
#include "Blueprint/WidgetTree.h"
#include "UI/MMSlot.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMMInventoryWidget::NativeConstruct()
{
	SetType(ESlotType::ST_InventoryEquipment);

	if (BTN_Equipment)
	{
		BTN_Equipment->OnClicked.AddDynamic(this, &UMMInventoryWidget::SetEquipmentType);
	}

	if (BTN_Consumable)
	{
		BTN_Consumable->OnClicked.AddDynamic(this, &UMMInventoryWidget::SetConsumableType);
	}

	if (BTN_Other)
	{
		BTN_Other->OnClicked.AddDynamic(this, &UMMInventoryWidget::SetOtherType);
	}
}

void UMMInventoryWidget::Init()
{
	Slots.Init(nullptr, 30);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		UMMSlot* InvSlot = Cast<UMMSlot>(Widget);
		if (InvSlot)
		{
			InvSlot->SetOwningActor(OwningActor);
			InvSlot->SetType(InventorySlotType);
			InvSlot->Init();
			Slots[InvSlot->SlotIndex] = InvSlot;
		}
	}
}

void UMMInventoryWidget::UpdateInventorySlot()
{
	// 슬롯 초기화
	for (const auto& InvSlot : Slots)
	{
		InvSlot->SetType(InventorySlotType);
		InvSlot->UpdateSlot();
	}
}

void UMMInventoryWidget::UpdateInventoryGold()
{
	UE_LOG(LogTemp, Warning, TEXT("GoldUpdate"));
}

void UMMInventoryWidget::SetEquipmentType()
{
	SetType(ESlotType::ST_InventoryEquipment);
}

void UMMInventoryWidget::SetConsumableType()
{
	SetType(ESlotType::ST_InventoryConsumable);
}

void UMMInventoryWidget::SetOtherType()
{
	SetType(ESlotType::ST_InventoryOther);
}

void UMMInventoryWidget::SetType(ESlotType Type)
{
	InventorySlotType = Type;

	UpdateInventorySlot();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMSlot.h"
#include "Item/MMItemData.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"


void UMMSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotUpdateActions.Add(ESlotType::ST_InventoryEquipment, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateEquipmentSlot)));
	SlotUpdateActions.Add(ESlotType::ST_InventoryConsumable, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateConsumableSlot)));
	SlotUpdateActions.Add(ESlotType::ST_InventoryOther, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateOtherSlot)));
}

void UMMSlot::Init()
{
	UpdateSlot();
}

void UMMSlot::SetType(ESlotType Type)
{
	SlotType = Type;
}

void UMMSlot::UpdateSlot()
{
	// 슬롯 타입에 따라 실행되는 함수 설정
	SlotUpdateActions[SlotType].SlotDelegate.ExecuteIfBound();
}

void UMMSlot::UpdateEquipmentSlot()
{
	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);

	if (InvPlayer)
	{
		// 인벤토리(Equipment)를 가져옵니다.
		TArray<UMMInventoryItem*> InventoryItems = InvPlayer->GetInventoryComponent()->GetEquipmentItems();
		
		// 현재 Slot의 인덱스가 유효한지 체크합니다.
		if (InventoryItems.IsValidIndex(SlotIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지 확인합니다.
			if (IsValid(InventoryItems[SlotIndex]))
			{
				// 존재하는 경우 아이템의 텍스쳐와 수량을 반영해주도록 합니다. (장비는 수량 표시 X)
				IMG_Item->SetBrushFromTexture(InventoryItems[SlotIndex]->ItemData->ItemTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
			else
			{
				// 존재하지 않는 경우 빈 칸으로 표시합니다.
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
		}
	}
}

void UMMSlot::UpdateConsumableSlot()
{
	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);

	if (InvPlayer)
	{
		// 인벤토리(Consumable)를 가져옵니다.
		TArray<UMMInventoryItem*> InventoryItems = InvPlayer->GetInventoryComponent()->GetConsumableItems();

		// 현재 Slot의 인덱스가 유효한지 체크합니다.
		if (InventoryItems.IsValidIndex(SlotIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지 확인합니다.
			if (IsValid(InventoryItems[SlotIndex]))
			{
				// 존재하는 경우 아이템의 텍스쳐와 수량을 반영해주도록 합니다. (소비는 수량 표시 O)
				IMG_Item->SetBrushFromTexture(InventoryItems[SlotIndex]->ItemData->ItemTexture);
				TXT_Quantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), InventoryItems[SlotIndex]->ItemQuantity)));
			}
			else
			{
				// 존재하지 않는 경우 빈 칸으로 표시합니다.
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
		}
	}
}

void UMMSlot::UpdateOtherSlot()
{
	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);

	if (InvPlayer)
	{
		// 인벤토리(Consumable)를 가져옵니다.
		TArray<UMMInventoryItem*> InventoryItems = InvPlayer->GetInventoryComponent()->GetOtherItems();

		// 현재 Slot의 인덱스가 유효한지 체크합니다.
		if (InventoryItems.IsValidIndex(SlotIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지 확인합니다.
			if (IsValid(InventoryItems[SlotIndex]))
			{
				// 존재하는 경우 아이템의 텍스쳐와 수량을 반영해주도록 합니다. (기타는 수량 표시 O)
				IMG_Item->SetBrushFromTexture(InventoryItems[SlotIndex]->ItemData->ItemTexture);
				TXT_Quantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), InventoryItems[SlotIndex]->ItemQuantity)));
			}
			else
			{
				// 존재하지 않는 경우 빈 칸으로 표시합니다.
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));
			}
		}
	}
}

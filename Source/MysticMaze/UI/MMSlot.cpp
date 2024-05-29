﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMSlot.h"
#include "Item/MMItemData.h"
#include "Item/MMWeaponItemData.h"
#include "Item/MMPotionItemData.h"
#include "Item/MMManaStoneItemData.h"
#include "GameData/MMCharacterStat.h"
#include "GameData/MMEnums.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"
#include "Interface/MMSkillInterface.h"
#include "Player/MMSkillComponent.h"
#include "Skill/MMSkillData.h"
#include "Skill/MMSkillBase.h"
#include "UI/MMDragSlot.h"

#include "UI/ToolTip/MMToolTip.h"
#include "UI/ToolTip/MMEquipmentToolTip.h"
#include "UI/ToolTip/MMConsumableToolTip.h"
#include "UI/ToolTip/MMOtherToolTip.h"
#include "UI/ToolTip/MMSkillToolTip.h"

#include "GameFramework/Character.h"
#include "Components/SlateWrapperTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableTextBox.h"


void UMMSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UMMSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 우클릭 입력이 들어온 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
		if (!InvPlayer) return Reply.NativeReply;

		switch (SlotType)
		{
		case ESlotType::ST_InventoryConsumable:
			// 소비 아이템을 사용합니다.
			InvPlayer->GetInventoryComponent()->UseItem(SlotIndex, SlotType);
			UpdateSlot();
			break;

		case ESlotType::ST_Equipment:
			// 무기를 해제합니다.
			InvPlayer->GetInventoryComponent()->UnEquipItem();
			break;
		}
	}
	// 좌클릭 입력이 들어온 경우
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// 정보 체크용 변수
		bool Success = false;

		// 해당 슬롯에 아이템 정보가 존재하는지 체크합니다.
		IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
		IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(OwningActor);
		if (InvPlayer && SkillPlayer)
		{
			switch (SlotType)
			{
			case ESlotType::ST_InventoryEquipment:
				if (IsValid(InvPlayer->GetInventoryComponent()->GetEquipmentItems()[SlotIndex]))
				{
					Success = true;
				}
				break;
			case ESlotType::ST_InventoryConsumable:
				if (IsValid(InvPlayer->GetInventoryComponent()->GetConsumableItems()[SlotIndex]))
				{
					Success = true;
				}
				break;
			case ESlotType::ST_InventoryOther:
				if (IsValid(InvPlayer->GetInventoryComponent()->GetOtherItems()[SlotIndex]))
				{
					Success = true;
				}
				break;
			case ESlotType::ST_SkillSlot:
				if (IsValid(SkillPlayer->GetSkillComponent()->GetSkillList()[SlotIndex]))
				{
					Success = true;
				}
				break;
			}

			if (Success)
			{
				Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			}
		}	
	}
	
	return Reply.NativeReply;
}

void UMMSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UMMDragSlot* Operation = NewObject<UMMDragSlot>();
		OutOperation = Operation;

		// 슬롯과 슬롯 타입을 지정합니다.
		Operation->PrevSlotIndex = SlotIndex;
		Operation->SlotType = SlotType;
		
		// Drag 위젯을 생성합니다.
		if (DragWidgetClass)
		{
			UMMSlot* DragWidget = CreateWidget<UMMSlot>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				// 생성된 위젯을 초기화해줍니다.
				DragWidget->SlotType = SlotType;
				DragWidget->SetOwningActor(OwningActor);
				DragWidget->SlotIndex = SlotIndex;
				DragWidget->Init();

				Operation->DefaultDragVisual = DragWidget;
			}
		}
	}
}

bool UMMSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UMMDragSlot* Operation = Cast<UMMDragSlot>(InOperation);

	if (Operation)
	{
		// 같은 타입의 슬롯인 경우
		if (Operation->SlotType == SlotType && Operation->SlotType != ESlotType::ST_SkillSlot)
		{
			// Operation에 저장된 PrevSlotIndex 위치의 아이템을 현재 SlotIndex의 아이템과 교체합니다.
			IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
			if (InvPlayer)
			{
				InvPlayer->GetInventoryComponent()->SwapItem(Operation->PrevSlotIndex, SlotIndex, Operation->SlotType, SlotType);
			}
			return true;
		}
		// 다른 타입의 슬롯인 경우 (소비 슬롯 -> 퀵슬롯[포션])
		else if (Operation->SlotType == ESlotType::ST_InventoryConsumable && SlotType == ESlotType::ST_PotionSlot)
		{
			// Operation에 저장된 PrevSlotIndex 위치의 아이템을 현재 SlotIndex에 저장합니다.
			IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
			if (InvPlayer)
			{
				InvPlayer->GetInventoryComponent()->SetQuickSlot(Operation->SlotType, Operation->PrevSlotIndex, SlotIndex);
			}
		}
		// 다른 타입의 슬롯인 경우 (장비 슬롯 -> 장비창)
		else if (Operation->SlotType == ESlotType::ST_InventoryEquipment && SlotType == ESlotType::ST_Equipment)
		{
			// Operation에 저장된 PrevSlotIndex 위치의 아이템을 장착합니다.
			IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
			if (InvPlayer)
			{
				InvPlayer->GetInventoryComponent()->EquipItem(Operation->PrevSlotIndex);
			}
		}
		// 다른 타입의 슬롯인 경우 (스킬 슬롯 -> 퀵슬롯[스킬])
		else if (Operation->SlotType == ESlotType::ST_SkillSlot && SlotType == ESlotType::ST_SkillQuickSlot)
		{
			// Operation에 저장된 PrevSlotIndex 위치의 스킬을 장착합니다.
			IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(OwningActor);
			if (SkillPlayer)
			{
				SkillPlayer->GetSkillComponent()->SetQuickSlot(Operation->SlotType, Operation->PrevSlotIndex, SlotIndex);
			}
		}
	}

	return false;
}

void UMMSlot::Init()
{
	for (const auto& ToolTipClass : ToolTipClassMap)
	{
		if (ToolTipClass.Value)
		{
			ToolTipMaps.Add(ToolTipClass.Key, CreateWidget<UMMToolTip>(GetWorld(), ToolTipClass.Value));
		}
	}

	SlotUpdateActions.Add(ESlotType::ST_InventoryEquipment, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateEquipmentSlot)));
	SlotUpdateActions.Add(ESlotType::ST_InventoryConsumable, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateConsumableSlot)));
	SlotUpdateActions.Add(ESlotType::ST_InventoryOther, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateOtherSlot)));
	SlotUpdateActions.Add(ESlotType::ST_SkillSlot, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateSkillSlot)));
	SlotUpdateActions.Add(ESlotType::ST_SkillQuickSlot, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateSkillQuickSlot)));
	SlotUpdateActions.Add(ESlotType::ST_PotionSlot, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdatePotionSlot)));
	SlotUpdateActions.Add(ESlotType::ST_Equipment, FUpdateSlotDelegateWrapper(FOnUpdateSlotDelegate::CreateUObject(this, &UMMSlot::UpdateEquipment)));

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

				// SetToolTip
				if (ToolTipMaps.Contains(ESlotType::ST_InventoryEquipment) && IsValid(ToolTipMaps[SlotType]))
				{
					SetEquipmentToolTip(ToolTipMaps[SlotType], InventoryItems[SlotIndex]->ItemData);
					IMG_Item->SetToolTip(ToolTipMaps[SlotType]);
				}
			}
			else
			{
				// 존재하지 않는 경우 빈 칸으로 표시합니다.
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));

				// SetToolTip
				IMG_Item->SetToolTip(nullptr);
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

				// SetToolTip
				if (ToolTipMaps.Contains(ESlotType::ST_InventoryConsumable) && IsValid(ToolTipMaps[SlotType]))
				{
					SetConsumableToolTip(ToolTipMaps[SlotType], InventoryItems[SlotIndex]->ItemData);
					IMG_Item->SetToolTip(ToolTipMaps[SlotType]);
				}
			}
			else
			{
				// 존재하지 않는 경우 빈 칸으로 표시합니다.
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));

				// SetToolTip
				IMG_Item->SetToolTip(nullptr);
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
			
				// SetToolTip
				if (ToolTipMaps.Contains(ESlotType::ST_InventoryOther) && IsValid(ToolTipMaps[SlotType]))
				{
					SetOtherToolTip(ToolTipMaps[SlotType], InventoryItems[SlotIndex]->ItemData);
					IMG_Item->SetToolTip(ToolTipMaps[SlotType]);
				}
			}
			else
			{
				// 존재하지 않는 경우 빈 칸으로 표시합니다.
				IMG_Item->SetBrushFromTexture(DefaultTexture);
				TXT_Quantity->SetText(FText::FromString(TEXT("")));

				// SetToolTip
				IMG_Item->SetToolTip(nullptr);
			}
		}
	}
}

void UMMSlot::UpdateSkillSlot()
{
	IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(OwningActor);

	if (SkillPlayer)
	{
		const TArray<UMMSkillBase*>& SkillList = SkillPlayer->GetSkillComponent()->GetSkillList();

		if (SkillList.IsValidIndex(SlotIndex) && IsValid(SkillList[SlotIndex]))
		{
			IMG_Item->SetBrushFromTexture(SkillList[SlotIndex]->GetSkillData()->SkillIcon);
			TXT_Quantity->SetText(FText::FromString(TEXT("")));

			// SetToolTip
			if (ToolTipMaps.Contains(ESlotType::ST_SkillSlot) && IsValid(ToolTipMaps[SlotType]))
			{
				SetSkillToolTip(ToolTipMaps[SlotType], SkillList[SlotIndex]);
				IMG_Item->SetToolTip(ToolTipMaps[SlotType]);
			}
		}
		else
		{
			IMG_Item->SetBrushFromTexture(DefaultTexture);
			TXT_Quantity->SetText(FText::FromString(TEXT("")));

			// SetToolTip
			IMG_Item->SetToolTip(nullptr);
		}
	}
}

void UMMSlot::UpdateSkillQuickSlot()
{
	IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(OwningActor);

	if (SkillPlayer)
	{
		const TArray<UMMSkillBase*>& SkillList = SkillPlayer->GetSkillComponent()->GetSkillQuickSlot();

		if (SkillList.IsValidIndex(SlotIndex) && IsValid(SkillList[SlotIndex]))
		{
			IMG_Item->SetBrushFromTexture(SkillList[SlotIndex]->GetSkillData()->SkillIcon);
			TXT_Quantity->SetText(FText::FromString(TEXT("")));
		}
		else
		{
			IMG_Item->SetBrushFromTexture(DefaultTexture);
			TXT_Quantity->SetText(FText::FromString(TEXT("")));
		}
	}
}

void UMMSlot::UpdatePotionSlot()
{
	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);

	if (InvPlayer)
	{
		// 포션 퀵슬롯을 가져옵니다.
		TArray<UMMInventoryItem*>& QuickSlots = InvPlayer->GetInventoryComponent()->GetPotionQuickSlots();

		if (QuickSlots.IsValidIndex(SlotIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지 확인합니다.
			if (IsValid(QuickSlots[SlotIndex]))
			{
				if (QuickSlots[SlotIndex]->ItemQuantity > 0)
				{
					// 존재하는 경우 아이템의 텍스쳐와 수량을 반영해주도록 합니다.
					IMG_Item->SetBrushFromTexture(QuickSlots[SlotIndex]->ItemData->ItemTexture);
					TXT_Quantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), QuickSlots[SlotIndex]->ItemQuantity)));
				}
				else
				{
					// 슬롯을 초기화 합니다.
					QuickSlots[SlotIndex] = nullptr;

					// 슬롯을 빈 칸으로 표시합니다.
					IMG_Item->SetBrushFromTexture(DefaultTexture);
					TXT_Quantity->SetText(FText::FromString(TEXT("")));
				}
				
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

void UMMSlot::UpdateEquipment()
{
	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);

	if (InvPlayer)
	{
		UMMInventoryItem* EquipmentItem = InvPlayer->GetInventoryComponent()->GetEquipmentItem();
		if (IsValid(EquipmentItem))
		{
			// 존재하는 경우 아이템의 텍스쳐를 반영해줍니다.
			IMG_Item->SetBrushFromTexture(EquipmentItem->ItemData->ItemTexture);
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

void UMMSlot::SetEquipmentToolTip(UMMToolTip* EquipmentToolTipWidget, UMMItemData* ItemData)
{
	UMMEquipmentToolTip* EquipmentToolTip = Cast<UMMEquipmentToolTip>(EquipmentToolTipWidget);
	UMMWeaponItemData* WeaponItemData = Cast<UMMWeaponItemData>(ItemData);
	if (EquipmentToolTip && WeaponItemData)
	{
		// 이름 및 설명
		EquipmentToolTip->TXT_ItemName->SetText(FText::FromString(WeaponItemData->ItemName));
		EquipmentToolTip->TXT_ItemType->SetText(FText::FromString(TEXT("무기")));

		FString ClassName;
		switch (WeaponItemData->PurchaseableClass)
		{
		case EClassType::CT_Warrior:
			ClassName = TEXT("전사");
			break;
		case EClassType::CT_Archer:
			ClassName = TEXT("궁수");
			break;
		case EClassType::CT_Mage:
			ClassName = TEXT("마법사");
			break;
		}
		EquipmentToolTip->TXT_WeaponType->SetText(FText::FromString(ClassName));

		// 추가 스탯
		EquipmentToolTip->TXT_STR->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponItemData->WeaponStat.STR)));
		EquipmentToolTip->TXT_DEX->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponItemData->WeaponStat.DEX)));
		EquipmentToolTip->TXT_CON->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponItemData->WeaponStat.CON)));
		EquipmentToolTip->TXT_INT->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponItemData->WeaponStat.INT)));

		// 구매 및 판매 가격
		EquipmentToolTip->TXT_PurchasePrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponItemData->ItemPurchasePrice)));
		EquipmentToolTip->TXT_SalePrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponItemData->ItemSalePrice)));
 	}
}

void UMMSlot::SetConsumableToolTip(UMMToolTip* ConsumableToolTipWidget, UMMItemData* ItemData)
{
	UMMConsumableToolTip* ConsumableToolTip = Cast<UMMConsumableToolTip>(ConsumableToolTipWidget);
	UMMPotionItemData* ConsumeableItemData = Cast<UMMPotionItemData>(ItemData);

	if (ConsumableToolTip && ConsumeableItemData)
	{
		// 이름 및 설명
		ConsumableToolTip->TXT_ItemName->SetText(FText::FromString(ConsumeableItemData->ItemName));
		ConsumableToolTip->TXT_ItemType->SetText(FText::FromString(TEXT("포션")));

		// 사용 효과
		switch (ConsumeableItemData->PotionType)
		{
		case EPotionType::PT_Hp:
			ConsumableToolTip->TXT_HpPercent->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ConsumeableItemData->Percent)));
			ConsumableToolTip->TXT_MpPercent->SetText(FText::FromString(TEXT("0.0")));
			break;

		case EPotionType::PT_Mp:
			ConsumableToolTip->TXT_HpPercent->SetText(FText::FromString(TEXT("0.0")));
			ConsumableToolTip->TXT_MpPercent->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ConsumeableItemData->Percent)));
			break;
		}

		// 구매 및 판매 가격
		ConsumableToolTip->TXT_PurchasePrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), ConsumeableItemData->ItemPurchasePrice)));
		ConsumableToolTip->TXT_SalePrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), ConsumeableItemData->ItemSalePrice)));
	}
}

void UMMSlot::SetOtherToolTip(UMMToolTip* OtherToolTipWidget, UMMItemData* ItemData)
{
	UMMOtherToolTip* OtherToolTip = Cast<UMMOtherToolTip>(OtherToolTipWidget);
	UMMManaStoneItemData* OtherItemData = Cast<UMMManaStoneItemData>(ItemData);

	if (OtherToolTip && OtherItemData)
	{
		// 이름 및 설명
		OtherToolTip->TXT_ItemName->SetText(FText::FromString(OtherItemData->ItemName));
		OtherToolTip->TXT_ItemType->SetText(FText::FromString(TEXT("마나스톤")));

		// 판매 가격
		OtherToolTip->TXT_SalePrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), OtherItemData->ItemSalePrice)));
	}
}

void UMMSlot::SetSkillToolTip(UMMToolTip* SkillToolTipWidget, UMMSkillBase* Skill)
{
	UMMSkillToolTip* SkillToolTip = Cast<UMMSkillToolTip>(SkillToolTipWidget);
	UMMSkillData* SkillData = Cast<UMMSkillData>(Skill->GetSkillData());

	if (SkillToolTip && SkillData)
	{
		SkillToolTip->TXT_ItemName->SetText(FText::FromString(SkillData->SkillName));
		SkillToolTip->TXT_RequireLevel->SetText(FText::FromString(FString::Printf(TEXT("%d"),SkillData->RequiredLevel)));
		SkillToolTip->TXTB_SkillDesc->SetText(FText::FromString(SkillData->SkillDescription));
		SkillToolTip->TXT_SkillCoolTime->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillData->CoolTime)));
		SkillToolTip->TXT_ManaCost->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), SkillData->ManaCost)));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPC/MMBlacksmithWidget.h"
#include "UI/MMSlot.h"
#include "UI/MMCustomButton.h"
#include "UI/MMCustomStat.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"
#include "Item/MMItemData.h"
#include "Item/MMWeaponItemData.h"
#include "NPC/MMBlacksmithNPC.h"


#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMMBlacksmithWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMBlacksmithWidget::Init(AActor* InPlayer)
{
	if (!InPlayer) return;

	TempPlayer = InPlayer;

	// 슬롯 초기 타입 지정
	SetInvType(ESlotType::ST_InventoryEquipment);
	SetShopType(ESlotType::ST_ShopWarriorWeapon);

	// 상점 및 인벤토리 슬롯 초기화
	ShopSlots.Init(nullptr, 4);
	ShopPurchaseButtons.Init(nullptr, 4);
	ShopItemStats.Init(nullptr, 4);
	InvSlots.Init(nullptr, 30);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		// 위젯 타입이 슬롯인 경우
		if (UMMSlot* ItemSlot = Cast<UMMSlot>(Widget))
		{
			// 상점 슬롯인 경우
			if (Widget->GetName().Contains(TEXT("ShopSlot")))
			{
				// 상인을 소유자로 지정하고 초기화합니다.
				ItemSlot->SetOwningActor(OwningActor);
				ItemSlot->SetType(ShopSlotType);
				ItemSlot->Init();
				ShopSlots[ItemSlot->SlotIndex] = ItemSlot;
			}
			else if (Widget->GetName().Contains(TEXT("InvSlot")))
			{
				// 플레이어를 소유자로 지정하고 초기화합니다.
				ItemSlot->SetOwningActor(InPlayer);
				ItemSlot->SetType(InventorySlotType);
				ItemSlot->Init();
				InvSlots[ItemSlot->SlotIndex] = ItemSlot;
			}
		}
		// 텍스트 블록인 경우
		else if (UTextBlock* TextBlock = Cast<UTextBlock>(Widget))
		{
			// 아이템 이름 텍스트인 경우
			if (Widget->GetName().Contains(TEXT("TXT_ItemName")))
			{
				// 아이템 이름 배열에 추가합니다.
				ShopItemNames.Add(TextBlock);
			}
			// 마석 수량 텍스트인 경우
			else if (Widget->GetName().Contains(TEXT("TXT_Amount")))
			{
				// 마석 수량 배열에 추가합니다.
				ShopItemAmounts.Add(TextBlock);
			}
		}
		// 버튼인 경우
		else if (UMMCustomButton* Button = Cast<UMMCustomButton>(Widget))
		{
			// 상인을 소유자로 지정하고 아이템 구매 버튼 배열에 추가합니다.
			Button->SetOwningActor(OwningActor);
			Button->SetType(ESlotType::ST_ShopPotion);
			Button->Init();

			ShopPurchaseButtons[Button->ButtonIndex] = Button;
		}
		// 커스텀 스탯인 경우
		else if (UMMCustomStat* Stat = Cast<UMMCustomStat>(Widget))
		{
			// 상인을 소유자로 지정하고 아이템 스탯 배열에 추가합니다.
			Stat->SetOwningActor(OwningActor);
			Stat->SetType(EClassType::CT_Warrior);
			Stat->Init();

			ShopItemStats[Stat->WeaponIndex] = Stat;
		}
	}

	// 버튼 연결
	if (BTN_Equipment)
		BTN_Equipment->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::SetEquipmentType);

	if (BTN_Consumable)
		BTN_Consumable->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::SetConsumableType);

	if (BTN_Other)
		BTN_Other->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::SetOtherType);

	if (BTN_Warrior)
		BTN_Warrior->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::SetWarriorType);

	if (BTN_Archer)
		BTN_Archer->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::SetArcherType);

	if (BTN_Mage)
		BTN_Mage->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::SetMageType);

	if (BTN_Close)
		BTN_Close->OnClicked.AddDynamic(this, &UMMBlacksmithWidget::Close);

	// 상점 초기화
	UpdateShopSlot();
	UpdateGold();
}

void UMMBlacksmithWidget::SetEquipmentType()
{
	SetInvType(ESlotType::ST_InventoryEquipment);
}

void UMMBlacksmithWidget::SetConsumableType()
{
	SetInvType(ESlotType::ST_InventoryConsumable);
}

void UMMBlacksmithWidget::SetOtherType()
{
	SetInvType(ESlotType::ST_InventoryOther);
}

void UMMBlacksmithWidget::SetWarriorType()
{
	SetShopType(ESlotType::ST_ShopWarriorWeapon);
}

void UMMBlacksmithWidget::SetArcherType()
{
	SetShopType(ESlotType::ST_ShopArcherWeapon);
}

void UMMBlacksmithWidget::SetMageType()
{
	SetShopType(ESlotType::ST_ShopMageWeapon);
}

void UMMBlacksmithWidget::Close()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		ShopNpc->CloseShop();
	}
}

void UMMBlacksmithWidget::UpdateShopSlot()
{
	// 슬롯 초기화
	for (const auto& ShopSlot : ShopSlots)
	{
		ShopSlot->SetType(ShopSlotType);
		ShopSlot->UpdateSlot();
	}

	// 스탯 및 아이템 정보 초기화
	for (const auto& Stat : ShopItemStats)
	{
		switch (ShopSlotType)
		{
		case ESlotType::ST_ShopWarriorWeapon:
			Stat->SetType(EClassType::CT_Warrior);
			Stat->UpdateStat();

			UpdateWarriorShop();
			break;

		case ESlotType::ST_ShopArcherWeapon:
			Stat->SetType(EClassType::CT_Archer);
			Stat->UpdateStat();

			UpdateArcherShop();
			break;

		case ESlotType::ST_ShopMageWeapon:
			Stat->SetType(EClassType::CT_Mage);
			Stat->UpdateStat();

			UpdateMageShop();
			break;
		}
	}

	// 버튼 초기화
	for (const auto& Button : ShopPurchaseButtons)
	{
		Button->SetType(ShopSlotType);
		Button->UpdateText();
	}
}

void UMMBlacksmithWidget::UpdateInventorySlot()
{
	// 슬롯 초기화
	for (const auto& InvSlot : InvSlots)
	{
		InvSlot->SetType(InventorySlotType);
		InvSlot->UpdateSlot();
	}
}

void UMMBlacksmithWidget::UpdateGold()
{
	if (TempPlayer)
	{
		IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(TempPlayer);
		if (InvPlayer)
		{
			TXT_CurrentGold->SetText(FText::FromString(FString::Printf(TEXT("%d"), InvPlayer->GetInventoryComponent()->GetCurrentGold())));
		}
	}
}

void UMMBlacksmithWidget::UpdateWarriorShop()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetWarriorItems();
		// 아이템 이름 초기화
		for (int i = 0; i < ShopItemNames.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				ShopItemNames[i]->SetText(FText::FromString(FString::Printf(TEXT("%s"), *ShopItems[i]->ItemData->ItemName)));
			}
		}

		// 마석 수량 초기화
		for (int i = 0; i < ShopItemAmounts.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				UMMWeaponItemData* WeaponData = Cast<UMMWeaponItemData>(ShopItems[i]->ItemData);
				if (WeaponData)
				{
					ShopItemAmounts[i]->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponData->ItemMaterialQuantity)));
				}
			}
		}
	}
}

void UMMBlacksmithWidget::UpdateArcherShop()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetArcherItems();
		// 아이템 이름 초기화
		for (int i = 0; i < ShopItemNames.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				ShopItemNames[i]->SetText(FText::FromString(FString::Printf(TEXT("%s"), *ShopItems[i]->ItemData->ItemName)));
			}
		}

		// 마석 수량 초기화
		for (int i = 0; i < ShopItemAmounts.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				UMMWeaponItemData* WeaponData = Cast<UMMWeaponItemData>(ShopItems[i]->ItemData);
				if (WeaponData)
				{
					ShopItemAmounts[i]->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponData->ItemMaterialQuantity)));
				}
			}
		}
	}
}

void UMMBlacksmithWidget::UpdateMageShop()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetMageItems();
		// 아이템 이름 초기화
		for (int i = 0; i < ShopItemNames.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				ShopItemNames[i]->SetText(FText::FromString(FString::Printf(TEXT("%s"), *ShopItems[i]->ItemData->ItemName)));
			}
		}

		// 마석 수량 초기화
		for (int i = 0; i < ShopItemAmounts.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				UMMWeaponItemData* WeaponData = Cast<UMMWeaponItemData>(ShopItems[i]->ItemData);
				if (WeaponData)
				{
					ShopItemAmounts[i]->SetText(FText::FromString(FString::Printf(TEXT("%d"), WeaponData->ItemMaterialQuantity)));
				}
			}
		}
	}
}

void UMMBlacksmithWidget::SetShopType(ESlotType Type)
{
	ShopSlotType = Type;

	UpdateShopSlot();
}

void UMMBlacksmithWidget::SetInvType(ESlotType Type)
{
	InventorySlotType = Type;

	UpdateInventorySlot();
}

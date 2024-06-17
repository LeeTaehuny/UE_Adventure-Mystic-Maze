// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPC/MMShopWidget.h"
#include "UI/MMSlot.h"
#include "UI/MMCustomButton.h"
#include "NPC/MMShopNPC.h"
#include "Item/MMItemData.h"
#include "Item/MMPotionItemData.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMMShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMShopWidget::Init(AActor* InPlayer)
{
	if (!InPlayer) return;

	TempPlayer = InPlayer;

	SetType(ESlotType::ST_InventoryEquipment);

	// 상점 및 인벤토리 슬롯 초기화
	ShopSlots.Init(nullptr, 6);
	ShopPurchaseButtons.Init(nullptr, 6);
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
				ItemSlot->SetType(ESlotType::ST_ShopPotion);
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
			// 아이템 성능 텍스트인 경우
			else if (Widget->GetName().Contains(TEXT("TXT_Amount")))
			{
				// 아이템 성능 배열에 추가합니다.
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
	}

	// 버튼 연결
	if (BTN_Equipment)
		BTN_Equipment->OnClicked.AddDynamic(this, &UMMShopWidget::SetEquipmentType);

	if (BTN_Consumable)
		BTN_Consumable->OnClicked.AddDynamic(this, &UMMShopWidget::SetConsumableType);

	if (BTN_Other)
		BTN_Other->OnClicked.AddDynamic(this, &UMMShopWidget::SetOtherType);
	
	if (BTN_Close)
		BTN_Close->OnClicked.AddDynamic(this, &UMMShopWidget::Close);

	// 상점 초기화
	InitShopSlot();
	UpdateGold();
}

void UMMShopWidget::SetEquipmentType()
{
	SetType(ESlotType::ST_InventoryEquipment);
}

void UMMShopWidget::SetConsumableType()
{
	SetType(ESlotType::ST_InventoryConsumable);
}

void UMMShopWidget::SetOtherType()
{
	SetType(ESlotType::ST_InventoryOther);
}

void UMMShopWidget::UpdateInventorySlot()
{
	// 슬롯 초기화
	for (const auto& InvSlot : InvSlots)
	{
		InvSlot->SetType(InventorySlotType);
		InvSlot->UpdateSlot();
	}
}

void UMMShopWidget::UpdateGold()
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

void UMMShopWidget::InitShopSlot()
{
	// 슬롯 초기화
	for (const auto& ShopSlot : ShopSlots)
	{
		ShopSlot->UpdateSlot();
	}

	AMMShopNPC* ShopNpc = Cast<AMMShopNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetItems();
		// 아이템 이름 초기화
		for (int i = 0; i < ShopItemNames.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				ShopItemNames[i]->SetText(FText::FromString(FString::Printf(TEXT("%s"), *ShopItems[i]->ItemData->ItemName)));
			}
		}

		// 아이템 성능 초기화
		for (int i = 0; i < ShopItemAmounts.Num(); i++)
		{
			if (ShopItems.IsValidIndex(i) && IsValid(ShopItems[i]))
			{
				UMMPotionItemData* PotionData = Cast<UMMPotionItemData>(ShopItems[i]->ItemData);
				if (PotionData)
				{
					ShopItemAmounts[i]->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), PotionData->Percent)));
				}
			}
		}
	}
}

void UMMShopWidget::Close()
{
	// 위젯 닫기
	AMMShopNPC* ShopNpc = Cast<AMMShopNPC>(OwningActor);
	if (ShopNpc)
	{
		ShopNpc->CloseShop();
	}
}

void UMMShopWidget::SetType(ESlotType Type)
{
	InventorySlotType = Type;

	UpdateInventorySlot();
}

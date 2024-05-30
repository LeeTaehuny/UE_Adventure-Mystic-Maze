// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMCustomButton.h"
#include "NPC/MMShopNPC.h"
#include "NPC/MMBlacksmithNPC.h"
#include "Player/MMInventoryItem.h"
#include "Item/MMItemData.h"


#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMMCustomButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMCustomButton::Init()
{
	TextUpdateActions.Add(ESlotType::ST_ShopPotion, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdatePotionShopButton)));
	TextUpdateActions.Add(ESlotType::ST_ShopWarriorWeapon, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdateWeaponWarriorShopButton)));
	TextUpdateActions.Add(ESlotType::ST_ShopArcherWeapon, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdateWeaponArcherShopButton)));
	TextUpdateActions.Add(ESlotType::ST_ShopMageWeapon, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdateWeaponMageShopButton)));

	if (BTN_CustomButton)
		BTN_CustomButton->OnClicked.AddDynamic(this, &UMMCustomButton::Purchase);

	UpdateText();
}

void UMMCustomButton::SetType(ESlotType Type)
{
	ButtonType = Type;
}

void UMMCustomButton::UpdateText()
{
	TextUpdateActions[ButtonType].ButtonDelegate.ExecuteIfBound();
}

void UMMCustomButton::Purchase()
{
	switch (ButtonType)
	{
	case ESlotType::ST_ShopPotion:
		PurchaseItem(ButtonIndex);
		break;
		
	case ESlotType::ST_ShopWarriorWeapon:
	case ESlotType::ST_ShopArcherWeapon:
	case ESlotType::ST_ShopMageWeapon:
		PurchaseWeapon(ButtonIndex);
		break;
	}
}

void UMMCustomButton::UpdatePotionShopButton()
{
	AMMShopNPC* ShopNpc = Cast<AMMShopNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetItems();
		if (ShopItems.IsValidIndex(ButtonIndex) && IsValid(ShopItems[ButtonIndex]))
		{
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), ShopItems[ButtonIndex]->ItemData->ItemPurchasePrice)));
		}
	}
}

void UMMCustomButton::UpdateWeaponWarriorShopButton()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetWarriorItems();
		if (ShopItems.IsValidIndex(ButtonIndex) && IsValid(ShopItems[ButtonIndex]))
		{
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), ShopItems[ButtonIndex]->ItemData->ItemPurchasePrice)));
		}
	}
}

void UMMCustomButton::UpdateWeaponArcherShopButton()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetArcherItems();
		if (ShopItems.IsValidIndex(ButtonIndex) && IsValid(ShopItems[ButtonIndex]))
		{
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), ShopItems[ButtonIndex]->ItemData->ItemPurchasePrice)));
		}
	}
}

void UMMCustomButton::UpdateWeaponMageShopButton()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetMageItems();
		if (ShopItems.IsValidIndex(ButtonIndex) && IsValid(ShopItems[ButtonIndex]))
		{
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), ShopItems[ButtonIndex]->ItemData->ItemPurchasePrice)));
		}
	}
}

void UMMCustomButton::PurchaseItem(int32 Index)
{
	AMMShopNPC* ShopNpc = Cast<AMMShopNPC>(OwningActor);
	if (ShopNpc)
	{
		ShopNpc->Purchase(Index);
	}
}

void UMMCustomButton::PurchaseWeapon(int32 Index)
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);
	if (ShopNpc)
	{
		ShopNpc->Purchase(ButtonType, Index);
	}
}
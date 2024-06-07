// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMCustomStat.h"
#include "Item/MMItemData.h"
#include "Item/MMWeaponItemData.h"
#include "NPC/MMBlacksmithNPC.h"
#include "Player/MMInventoryItem.h"

#include "Components/TextBlock.h"

void UMMCustomStat::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMCustomStat::Init()
{
	StatUpdateActions.Add(EClassType::CT_Warrior, FUpdateStatDelegateWrapper(FOnUpdateStatDelegate::CreateUObject(this, &UMMCustomStat::WarriorWeapon)));
	StatUpdateActions.Add(EClassType::CT_Archer, FUpdateStatDelegateWrapper(FOnUpdateStatDelegate::CreateUObject(this, &UMMCustomStat::ArcherWeapon)));
	StatUpdateActions.Add(EClassType::CT_Mage, FUpdateStatDelegateWrapper(FOnUpdateStatDelegate::CreateUObject(this, &UMMCustomStat::MageWeapon)));

	UpdateStat();
}

void UMMCustomStat::SetType(EClassType Type)
{
	WeaponType = Type;
}

void UMMCustomStat::UpdateStat()
{
	StatUpdateActions[WeaponType].StatDelegate.ExecuteIfBound();
}

void UMMCustomStat::WarriorWeapon()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetWarriorItems();
	
		// 무기 인덱스가 유효한지 체크합니다.
		if (ShopItems.IsValidIndex(WeaponIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지, 무기 타입이 맞는지 확인합니다.
			if (IsValid(ShopItems[WeaponIndex]))
			{
				UMMWeaponItemData* ItemData = Cast<UMMWeaponItemData>(ShopItems[WeaponIndex]->ItemData);
				if (ItemData)
				{
					// 존재하는 경우 무기의 정보를 받아와 처리합니다.
					TXT_StatSTR->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.STR)));
					TXT_StatDEX->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.DEX)));
					TXT_StatCON->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.CON)));
					TXT_StatINT->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.INT)));
				}
			}
		}
	}
}

void UMMCustomStat::ArcherWeapon()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetArcherItems();

		// 무기 인덱스가 유효한지 체크합니다.
		if (ShopItems.IsValidIndex(WeaponIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지, 무기 타입이 맞는지 확인합니다.
			if (IsValid(ShopItems[WeaponIndex]))
			{
				UMMWeaponItemData* ItemData = Cast<UMMWeaponItemData>(ShopItems[WeaponIndex]->ItemData);
				if (ItemData)
				{
					// 존재하는 경우 무기의 정보를 받아와 처리합니다.
					TXT_StatSTR->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.STR)));
					TXT_StatDEX->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.DEX)));
					TXT_StatCON->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.CON)));
					TXT_StatINT->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.INT)));
				}
			}
		}
	}
}

void UMMCustomStat::MageWeapon()
{
	AMMBlacksmithNPC* ShopNpc = Cast<AMMBlacksmithNPC>(OwningActor);

	if (ShopNpc)
	{
		TArray<UMMInventoryItem*> ShopItems = ShopNpc->GetMageItems();

		// 무기 인덱스가 유효한지 체크합니다.
		if (ShopItems.IsValidIndex(WeaponIndex))
		{
			// 해당 슬롯에 아이템이 존재하는지, 무기 타입이 맞는지 확인합니다.
			if (IsValid(ShopItems[WeaponIndex]))
			{
				UMMWeaponItemData* ItemData = Cast<UMMWeaponItemData>(ShopItems[WeaponIndex]->ItemData);
				if (ItemData)
				{
					// 존재하는 경우 무기의 정보를 받아와 처리합니다.
					TXT_StatSTR->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.STR)));
					TXT_StatDEX->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.DEX)));
					TXT_StatCON->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.CON)));
					TXT_StatINT->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData->WeaponStat.INT)));
				}
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMCustomButton.h"
#include "NPC/MMShopNPC.h"
#include "NPC/MMBlacksmithNPC.h"
#include "NPC/MMClassTrainerNPC.h"
#include "Player/MMInventoryItem.h"
#include "Item/MMItemData.h"
#include "Skill/MMSkillData.h"



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
	TextUpdateActions.Add(ESlotType::ST_ClassSkillWarrior, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdateClassSkillWarrior)));
	TextUpdateActions.Add(ESlotType::ST_ClassSkillArcher, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdateClassSkillArcher)));
	TextUpdateActions.Add(ESlotType::ST_ClassSkillMage, FUpdateButtonDelegateWrapper(FOnUpdateButtonDelegate::CreateUObject(this, &UMMCustomButton::UpdateClassSkillMage)));

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
	if (!TextUpdateActions.Find(ButtonType)) return;

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

	case ESlotType::ST_ClassSkillWarrior:
	case ESlotType::ST_ClassSkillArcher:
	case ESlotType::ST_ClassSkillMage:
		PurchaseSkill(ButtonIndex);
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

void UMMCustomButton::UpdateClassSkillWarrior()
{
	AMMClassTrainerNPC* ShopNpc = Cast<AMMClassTrainerNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMSkillData*> SkillData = ShopNpc->GetWarriorSkillData();
		TMap<FString, int32> WarriorSkillMap = ShopNpc->GetWarriorSkillMap();
		int32 PlayerLevel = ShopNpc->GetPlayerLevel();

		// 만약 아직 배울 수 없는 스킬이라면?
		if (SkillData[ButtonIndex]->RequiredLevel > PlayerLevel)
		{
			// 버튼 비활성화하기
			BTN_CustomButton->SetIsEnabled(false);
			// 레벨 적어주기
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("레벨 제한 %d"), SkillData[ButtonIndex]->RequiredLevel)));
		}
		// 배울 수 있는 스킬이라면?
		else
		{
			// 전사 스킬 맵에서 해당 스킬의 이름이 존재하는지 찾기
			if (WarriorSkillMap.Find(SkillData[ButtonIndex]->GetName()))
			{
				// 현재 스킬 레벨 불러오기
				int32 SkillLevel = WarriorSkillMap[SkillData[ButtonIndex]->GetName()];

				// 만약 스킬이 최대 레벨이라면? - 1, 2, 3 존재.
				if (SkillLevel > 3)
				{
					// 버튼 비활성화하기
					BTN_CustomButton->SetIsEnabled(false);
					// 최대 치라고 적어주기
					TXT_Price->SetText(FText::FromString(TEXT("최대 강화")));
				}
				else
				{
					// 버튼 활성화하기
					BTN_CustomButton->SetIsEnabled(true);
					// 가격 보여주기
					TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), SkillData[ButtonIndex]->UpgradeGold[SkillLevel - 1])));
				}
			}
		}
	}
}

void UMMCustomButton::UpdateClassSkillArcher()
{
	AMMClassTrainerNPC* ShopNpc = Cast<AMMClassTrainerNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMSkillData*> SkillData = ShopNpc->GetArcherSkillData();
		TMap<FString, int32> ArcherSkillMap = ShopNpc->GetArcherSkillMap();
		int32 PlayerLevel = ShopNpc->GetPlayerLevel();

		// 만약 아직 배울 수 없는 스킬이라면?
		if (SkillData[ButtonIndex]->RequiredLevel > PlayerLevel)
		{
			// 버튼 비활성화하기
			BTN_CustomButton->SetIsEnabled(false);
			// 레벨 적어주기
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("레벨 제한 %d"), SkillData[ButtonIndex]->RequiredLevel)));
		}
		// 배울 수 있는 스킬이라면?
		else
		{
			// 전사 스킬 맵에서 해당 스킬의 이름이 존재하는지 찾기
			if (ArcherSkillMap.Find(SkillData[ButtonIndex]->GetName()))
			{
				// 현재 스킬 레벨 불러오기
				int32 SkillLevel = ArcherSkillMap[SkillData[ButtonIndex]->GetName()];

				// 만약 스킬이 최대 레벨이라면? - 1, 2, 3 존재.
				if (SkillLevel > 3)
				{
					// 버튼 비활성화하기
					BTN_CustomButton->SetIsEnabled(false);
					// 최대 치라고 적어주기
					TXT_Price->SetText(FText::FromString(TEXT("최대 강화")));
				}
				else
				{
					// 버튼 활성화하기
					BTN_CustomButton->SetIsEnabled(true);
					// 가격 보여주기
					TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), SkillData[ButtonIndex]->UpgradeGold[SkillLevel - 1])));
				}
			}
		}
	}
}

void UMMCustomButton::UpdateClassSkillMage()
{
	AMMClassTrainerNPC* ShopNpc = Cast<AMMClassTrainerNPC>(OwningActor);
	if (ShopNpc)
	{
		TArray<UMMSkillData*> SkillData = ShopNpc->GetMageSkillData();
		TMap<FString, int32> MageSkillMap = ShopNpc->GetMageSkillMap();
		int32 PlayerLevel = ShopNpc->GetPlayerLevel();

		// 만약 아직 배울 수 없는 스킬이라면?
		if (SkillData[ButtonIndex]->RequiredLevel > PlayerLevel)
		{
			// 버튼 비활성화하기
			BTN_CustomButton->SetIsEnabled(false);
			// 레벨 적어주기
			TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("레벨 제한 %d"), SkillData[ButtonIndex]->RequiredLevel)));
		}
		// 배울 수 있는 스킬이라면?
		else
		{
			// 전사 스킬 맵에서 해당 스킬의 이름이 존재하는지 찾기
			if (MageSkillMap.Find(SkillData[ButtonIndex]->GetName()))
			{
				// 현재 스킬 레벨 불러오기
				int32 SkillLevel = MageSkillMap[SkillData[ButtonIndex]->GetName()];

				// 만약 스킬이 최대 레벨이라면? - 1, 2, 3 존재.
				if (SkillLevel >= 3)
				{
					// 버튼 비활성화하기
					BTN_CustomButton->SetIsEnabled(false);
					// 최대 치라고 적어주기
					TXT_Price->SetText(FText::FromString(TEXT("최대 강화")));
				}
				else
				{
					// 버튼 활성화하기
					BTN_CustomButton->SetIsEnabled(true);
					// 가격 보여주기
					TXT_Price->SetText(FText::FromString(FString::Printf(TEXT("%d$"), SkillData[ButtonIndex]->UpgradeGold[SkillLevel])));
				}
			}
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

void UMMCustomButton::PurchaseSkill(int32 Index)
{
	AMMClassTrainerNPC* ShopNpc = Cast<AMMClassTrainerNPC>(OwningActor);
	if (ShopNpc)
	{
		ShopNpc->Purchase(ButtonType, Index);
	}
}

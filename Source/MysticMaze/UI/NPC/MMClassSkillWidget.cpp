// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPC/MMClassSkillWidget.h"
#include "UI/MMSlot.h"
#include "UI/MMCustomButton.h"
#include "NPC/MMClassTrainerNPC.h"
#include "Skill/MMSkillBase.h"
#include "Skill/MMSkillData.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"

void UMMClassSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMClassSkillWidget::Init(AActor* InPlayer, EClassType InPlayerType)
{
	if (!InPlayer) return;

	// 스킬 타입 지정
	switch (InPlayerType)
	{
	case EClassType::CT_Beginner:
		SkillType = ESlotType::ST_None;
		break;
	case EClassType::CT_Warrior:
		SkillType = ESlotType::ST_ClassSkillWarrior;
		break;
	case EClassType::CT_Archer:
		SkillType = ESlotType::ST_ClassSkillArcher;
		break;
	case EClassType::CT_Mage:
		SkillType = ESlotType::ST_ClassSkillMage;
		break;
	}

	TempPlayer = InPlayer;

	// 슬롯 및 커스텀 버튼 배열 초기화
	SkillSlots.Init(nullptr, 4);
	SkillUpgradeButtons.Init(nullptr, 4);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		// 위젯 타입이 슬롯인 경우
		if (UMMSlot* SkillSlot = Cast<UMMSlot>(Widget))
		{
			SkillSlot->SetOwningActor(OwningActor);
			SkillSlot->SetType(SkillType);
			SkillSlot->Init();
			SkillSlots[SkillSlot->SlotIndex] = SkillSlot;
		}
		// 위젯 타입이 텍스트 타입인 경우
		else if (UTextBlock* TextBlock = Cast<UTextBlock>(Widget))
		{
			// 스킬 레벨 텍스트라면?
			if (Widget->GetName().Contains(TEXT("TXT_SkillLevel")))
			{
				// 스킬 레벨 배열에 추가합니다.
				SkillLevels.Add(TextBlock);
			}
			// 스킬 이름 텍스트라면?
			else if (Widget->GetName().Contains(TEXT("TXT_SkillName")))
			{
				// 스킬 이름 배열에 추가합니다.
				SkillNames.Add(TextBlock);
			}
			// 스킬 쿨타임 텍스트라면?
			else if (Widget->GetName().Contains(TEXT("TXT_CoolTime")))
			{
				// 스킬 쿨타임 배열에 추가합니다.
				SkillCoolTimes.Add(TextBlock);
			}
			// 소모 마나량 텍스트라면?
			else if (Widget->GetName().Contains(TEXT("TXT_ManaCost")))
			{
				// 스킬 쿨타임 배열에 추가합니다.
				SkillManaCosts.Add(TextBlock);
			}
		}
		// 위젯 타입이 멀티 텍스트 박스 타입인 경우
		else if (UMultiLineEditableTextBox* MultiTextBox = Cast<UMultiLineEditableTextBox>(Widget))
		{
			SkillDescs.Add(MultiTextBox);
		}
		// 위젯 타입이 버튼인 경우
		else if (UMMCustomButton* Button = Cast<UMMCustomButton>(Widget))
		{
			// 상인을 소유자로 지정하고 아이템 구매 버튼 배열에 추가합니다.
			Button->SetOwningActor(OwningActor);
			Button->SetType(SkillType);
			Button->Init();

			SkillUpgradeButtons[Button->ButtonIndex] = Button;
		}
	}

	UpdateWidget();
}

void UMMClassSkillWidget::UpdateWidget()
{
	if (!TempPlayer) return;

	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(TempPlayer);
	if (StatPlayer)
	{
		// 스킬 타입 지정
		switch (StatPlayer->GetStatComponent()->GetClass())
		{
		case EClassType::CT_Beginner:
			return;
			break;
		case EClassType::CT_Warrior:
			SkillType = ESlotType::ST_ClassSkillWarrior;
			break;
		case EClassType::CT_Archer:
			SkillType = ESlotType::ST_ClassSkillArcher;
			break;
		case EClassType::CT_Mage:
			SkillType = ESlotType::ST_ClassSkillMage;
			break;
		}

		// 슬롯 초기화
		for (const auto& SkillSlot : SkillSlots)
		{
			SkillSlot->SetType(SkillType);
			SkillSlot->UpdateSlot();
		}

		// 버튼 초기화
		for (const auto& UpgradeButton : SkillUpgradeButtons)
		{
			UpgradeButton->SetType(SkillType);
			UpgradeButton->UpdateText();
		}
	}

	AMMClassTrainerNPC* NPC = Cast<AMMClassTrainerNPC>(OwningActor);
	if (NPC)
	{
		TArray<UMMSkillData*> SkillDataList;
		TMap<FString, int32> SkillMap;

		switch (SkillType)
		{
		case ESlotType::ST_ClassSkillWarrior:
			SkillDataList = NPC->GetWarriorSkillData();
			SkillMap = NPC->GetWarriorSkillMap();
			break;

		case ESlotType::ST_ClassSkillArcher:
			SkillDataList = NPC->GetArcherSkillData();
			SkillMap = NPC->GetArcherSkillMap();
			break;

		case ESlotType::ST_ClassSkillMage:
			SkillDataList = NPC->GetMageSkillData();
			SkillMap = NPC->GetMageSkillMap();
			break;

		}

		// 레벨 초기화
		for (int i = 0; i < SkillLevels.Num(); i++)
		{
			if (SkillDataList.IsValidIndex(i) && IsValid(SkillDataList[i]))
			{
				if (SkillMap.Find(SkillDataList[i]->GetName()))
				{
					SkillLevels[i]->SetText(FText::FromString(FString::Printf(TEXT("Lv %d"), SkillMap[SkillDataList[i]->GetName()])));
				}
			}
		}

		// 이름 초기화
		for (int i = 0; i < SkillNames.Num(); i++)
		{
			if (SkillDataList.IsValidIndex(i) && IsValid(SkillDataList[i]))
			{
				SkillNames[i]->SetText(FText::FromString(SkillDataList[i]->SkillName));
			}
		}

		// 설명 초기화
		for (int i = 0; i < SkillDescs.Num(); i++)
		{
			if (SkillDataList.IsValidIndex(i) && IsValid(SkillDataList[i]))
			{
				SkillDescs[i]->SetText(FText::FromString(SkillDataList[i]->SkillDescription));
			}
		}

		// 쿨타임 초기화
		for (int i = 0; i < SkillCoolTimes.Num(); i++)
		{
			if (SkillDataList.IsValidIndex(i) && IsValid(SkillDataList[i]))
			{
				SkillCoolTimes[i]->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillDataList[i]->CoolTime)));
			}
		}

		// 마나 초기화
		for (int i = 0; i < SkillManaCosts.Num(); i++)
		{
			if (SkillDataList.IsValidIndex(i) && IsValid(SkillDataList[i]))
			{
				SkillManaCosts[i]->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillDataList[i]->ManaCost)));
			}
		}
	}
}

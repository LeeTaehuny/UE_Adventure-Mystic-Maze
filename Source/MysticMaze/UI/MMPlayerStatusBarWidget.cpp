// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMPlayerStatusBarWidget.h"
#include "UI/MMSlot.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Player/MMSkillComponent.h"
#include "Skill/MMSkillBase.h"
#include "Skill/MMSkillData.h"

#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UMMPlayerStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < 4; i++)
	{
		FName CoolTimeText = *FString::Printf(TEXT("TXT_CoolTime%d"), i + 1);
		UTextBlock* CoolTimeTextBlockWidget = Cast<UTextBlock>(GetWidgetFromName(CoolTimeText));

		if (CoolTimeTextBlockWidget)
		{
			CoolTimeTextBlockWidget->SetVisibility(ESlateVisibility::Hidden);
			CoolTimeTexts.Add(CoolTimeTextBlockWidget);
		}

		FName CoolTimeProgressBar = *FString::Printf(TEXT("PB_CoolTimeRate%d"), i + 1);
		UProgressBar* CoolTimeProgressBarWidget = Cast<UProgressBar>(GetWidgetFromName(CoolTimeProgressBar));
		if (CoolTimeProgressBarWidget)
		{
			CoolTimeProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
			CoolTimeProgressBars.Add(CoolTimeProgressBarWidget);
		}
	}
}

void UMMPlayerStatusBarWidget::Init()
{
	SkillSlots.Init(nullptr, 4);
	PotionSlots.Init(nullptr, 2);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		UMMSlot* QuickSlot = Cast<UMMSlot>(Widget);
		if (QuickSlot)
		{
			if (QuickSlot->GetName().Contains(TEXT("SkillSlot")))
			{
				QuickSlot->SetOwningActor(OwningActor);
				QuickSlot->SetType(ESlotType::ST_SkillQuickSlot);
				QuickSlot->Init();
				SkillSlots[QuickSlot->SlotIndex] = QuickSlot;
			}
			else if (QuickSlot->GetName().Contains(TEXT("PotionSlot")))
			{
				QuickSlot->SetOwningActor(OwningActor);
				QuickSlot->SetType(ESlotType::ST_PotionSlot);
				QuickSlot->Init();
				PotionSlots[QuickSlot->SlotIndex] = QuickSlot;
			}
		}
	}

	IMMStatusInterface* PlayerCharacter = Cast<IMMStatusInterface>(OwningActor);

	if (PlayerCharacter)
	{
		// 스탯 컴포넌트를 불러옵니다.
		UMMStatComponent* StatComponent = PlayerCharacter->GetStatComponent();

		UpdateHpBar(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
		UpdateMpBar(StatComponent->GetCurrentMp(), StatComponent->GetMaxMp());
		UpdateExpBar(StatComponent->GetCurrentExp(), StatComponent->GetMaxExp());
	}

	for (int i = 0; i < 4; i++)
		UpdateSkillData(i);
}

void UMMPlayerStatusBarWidget::UpdateSkillSlot()
{
	for (const auto& SkillSlot : SkillSlots)
	{
		SkillSlot->UpdateSlot();
	}

	for (int i = 0; i < 4; i++)
		UpdateSkillData(i);
}

void UMMPlayerStatusBarWidget::UpdatePotionSlot()
{
	for (const auto& PotionSlot : PotionSlots)
	{
		PotionSlot->UpdateSlot();
	}
}

void UMMPlayerStatusBarWidget::UpdateHpBar(float CurrentHp, float MaxHp)
{
	TXT_MaxHp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxHp)));
	TXT_CurrentHp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHp)));
	TXT_HpPercent->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), (CurrentHp / MaxHp) * 100)));
	PB_HpBar->SetPercent(CurrentHp / MaxHp);
}

void UMMPlayerStatusBarWidget::UpdateMpBar(float CurrentMp, float MaxMp)
{
	TXT_MaxMp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxMp)));
	TXT_CurrentMp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentMp)));
	TXT_MpPercent->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), (CurrentMp / MaxMp) * 100)));
	PB_MpBar->SetPercent(CurrentMp / MaxMp);
}

void UMMPlayerStatusBarWidget::UpdateExpBar(float CurrentExp, float MaxExp)
{
	TXT_MaxExp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxExp)));
	TXT_CurrentExp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentExp)));
	TXT_ExpPercent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), (CurrentExp / MaxExp) * 100)));
	PB_ExpBar->SetPercent(CurrentExp / MaxExp);
}

void UMMPlayerStatusBarWidget::UpdateSkillData(int32 QuickSlotIndex)
{
	IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(OwningActor);
	if (!SkillPlayer) return;

	if (CoolTimeProgressBars.IsValidIndex(QuickSlotIndex) && IsValid(CoolTimeProgressBars[QuickSlotIndex]) &&
		CoolTimeTexts.IsValidIndex(QuickSlotIndex) && IsValid(CoolTimeTexts[QuickSlotIndex]))
	{
		UMMSkillBase* Skill = SkillPlayer->GetSkillComponent()->GetSkillQuickSlot()[QuickSlotIndex];

		if (IsValid(Skill))
		{
			// 위젯 활성화
			CoolTimeProgressBars[QuickSlotIndex]->SetVisibility(ESlateVisibility::Visible);
			CoolTimeTexts[QuickSlotIndex]->SetVisibility(ESlateVisibility::Visible);

			float Percent = Skill->GetCurrentCoolTime() / Skill->GetSkillData()->CoolTime;

			// 쿨타임 게이지 설정
			CoolTimeProgressBars[QuickSlotIndex]->SetPercent(Percent);
			// 쿨타임 시간 설정
			if (Skill->GetCurrentCoolTime() == Skill->GetSkillData()->CoolTime)
			{
				CoolTimeTexts[QuickSlotIndex]->SetText(FText::FromString(TEXT("")));
				// 위젯 비활성화
				CoolTimeProgressBars[QuickSlotIndex]->SetVisibility(ESlateVisibility::Hidden);
				CoolTimeTexts[QuickSlotIndex]->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				CoolTimeTexts[QuickSlotIndex]->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Skill->GetCurrentCoolTime())));
			}
		}
		else
		{
			CoolTimeProgressBars[QuickSlotIndex]->SetPercent(0.0f);
			CoolTimeTexts[QuickSlotIndex]->SetText(FText::FromString(TEXT("")));

			CoolTimeProgressBars[QuickSlotIndex]->SetVisibility(ESlateVisibility::Hidden);
			CoolTimeTexts[QuickSlotIndex]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
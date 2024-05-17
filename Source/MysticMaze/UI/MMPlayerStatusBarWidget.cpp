// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMPlayerStatusBarWidget.h"
#include "Blueprint/WidgetTree.h"
#include "UI/MMSlot.h"

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
			CoolTimeTexts.Add(CoolTimeTextBlockWidget);
		}

		FName CoolTimeProgressBar = *FString::Printf(TEXT("PB_CoolTimeRate%d"), i + 1);
		UProgressBar* CoolTimeProgressBarWidget = Cast<UProgressBar>(GetWidgetFromName(CoolTimeProgressBar));
		if (CoolTimeProgressBarWidget)
		{
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
				UE_LOG(LogTemp, Warning, TEXT("%s"), *QuickSlot->GetName());
				
				QuickSlot->SetOwningActor(OwningActor);
				QuickSlot->SetType(ESlotType::ST_SkillSlot);
				QuickSlot->Init();
				SkillSlots[QuickSlot->SlotIndex] = QuickSlot;
			}
			else if (QuickSlot->GetName().Contains(TEXT("PotionSlot")))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *QuickSlot->GetName());

				QuickSlot->SetOwningActor(OwningActor);
				QuickSlot->SetType(ESlotType::ST_PotionSlot);
				QuickSlot->Init();
				PotionSlots[QuickSlot->SlotIndex] = QuickSlot;
			}
		}
	}
}

void UMMPlayerStatusBarWidget::UpdateSkillSlot()
{
}

void UMMPlayerStatusBarWidget::UpdatePotionSlot()
{
	for (const auto& PotionSlot : PotionSlots)
	{
		PotionSlot->UpdateSlot();
	}
}

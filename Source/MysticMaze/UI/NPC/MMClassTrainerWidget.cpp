// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPC/MMClassTrainerWidget.h"
#include "Interface/MMWidgetChangeInterface.h"
#include "Interface/MMInventoryInterface.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMStatComponent.h"
#include "GameData/MMEnums.h"
#include "UI/NPC/MMClassSelectWidget.h"
#include "UI/NPC/MMClassSkillWidget.h"
#include "Components/WidgetSwitcher.h"
#include "NPC/MMClassTrainerNPC.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMMClassTrainerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Close)
		BTN_Close->OnClicked.AddDynamic(this, &UMMClassTrainerWidget::Close);
}

void UMMClassTrainerWidget::Init(AActor* InPlayer)
{
	if (!InPlayer) return;
	TempPlayer = InPlayer;

	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(InPlayer);
	if (StatPlayer)
	{
		// 위젯 초기화
		ClassSelectWidget->SetOwningActor(OwningActor);
		ClassSelectWidget->Init(InPlayer, this);
		ClassSkillWidget->SetOwningActor(OwningActor);
		ClassSkillWidget->Init(InPlayer, StatPlayer->GetStatComponent()->GetClass());

		// 플레이어의 직업이 없으면 직업 선택 화면으로 전환합니다.
		if (StatPlayer->GetStatComponent()->GetClass() == EClassType::CT_Beginner)
		{
			ChangeWidget(ClassSelectWidget);
		}
		// 플레이어의 직업이 있으면 스킬 업그레이드 화면으로 전환합니다.
		else
		{
			ChangeWidget(ClassSkillWidget);		
		}
	}

	UpdateGold();
}

void UMMClassTrainerWidget::SwitchClassSkillWidget()
{
	if (ClassSkillWidget)
	{
		ChangeWidget(ClassSkillWidget);
	}
}

void UMMClassTrainerWidget::UpdateWidget()
{
	ChangeWidget(TempWidget);

	// Gold Update
	UpdateGold();
}

void UMMClassTrainerWidget::UpdateGold()
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

void UMMClassTrainerWidget::Close()
{
	AMMClassTrainerNPC* NPC = Cast<AMMClassTrainerNPC>(OwningActor);
	if (NPC)
	{
		NPC->CloseShop();
	}
}

void UMMClassTrainerWidget::ChangeWidget(UUserWidget* NewWidget)
{
	if (!NewWidget) return;

	IMMWidgetChangeInterface* Widget = Cast<IMMWidgetChangeInterface>(NewWidget);
	if (Widget)
	{
		Widget->UpdateWidget();
	}

	WidgetSwitcher->SetActiveWidget(NewWidget);
	TempWidget = NewWidget;
}

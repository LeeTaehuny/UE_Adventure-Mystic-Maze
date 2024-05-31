// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPC/MMClassSelectWidget.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"
#include "UI/NPC/MMClassTrainerWidget.h"

#include "Components/Button.h"

void UMMClassSelectWidget::NativeConstruct()
{
}

void UMMClassSelectWidget::Init(AActor* InPlayer, UUserWidget* InMainWidget)
{
	if (!InPlayer) return;
	TempPlayer = InPlayer;
	MainWidget = InMainWidget;

	// 버튼 연동
	if (BTN_SelectWarrior)
		BTN_SelectWarrior->OnClicked.AddDynamic(this, &UMMClassSelectWidget::SelectWarrior);

	if (BTN_SelectArcher)
		BTN_SelectArcher->OnClicked.AddDynamic(this, &UMMClassSelectWidget::SelectArcher);

	if (BTN_SelectMage)
		BTN_SelectMage->OnClicked.AddDynamic(this, &UMMClassSelectWidget::SelectMage);

	// 위젯 업데이트
	UpdateWidget();
}

void UMMClassSelectWidget::UpdateWidget()
{
	if (TempPlayer)
	{
		// 플레이어의 현재 레벨이 전직이 가능한 레벨인지 체크하여 버튼을 On/Off합니다.
		IMMStatusInterface* StatCharacter = Cast<IMMStatusInterface>(TempPlayer);
		if (StatCharacter)
		{
			// 5보다 작은 경우 비활성화 합니다.
			if (StatCharacter->GetStatComponent()->GetCurrentLevel() < 5)
			{
				BTN_SelectWarrior->SetIsEnabled(false);
				BTN_SelectArcher->SetIsEnabled(false);
				BTN_SelectMage->SetIsEnabled(false);
			}
			// 5보다 높은 경우 활성화합니다.
			else
			{
				BTN_SelectWarrior->SetIsEnabled(true);
				BTN_SelectArcher->SetIsEnabled(true);
				BTN_SelectMage->SetIsEnabled(true);
			}
		}
	}
}

void UMMClassSelectWidget::SelectWarrior()
{
	if (TempPlayer)
	{
		// 전사로 전직합니다.
		IMMStatusInterface* StatCharacter = Cast<IMMStatusInterface>(TempPlayer);
		if (StatCharacter)
		{
			StatCharacter->GetStatComponent()->SetClass(EClassType::CT_Warrior);
		}

		// 메인 위젯의 창을 스킬 업그레이드 창으로 변경합니다.
		UMMClassTrainerWidget* Widget = Cast<UMMClassTrainerWidget>(MainWidget);
		if (Widget)
		{
			Widget->SwitchClassSkillWidget();
		}
	}
}

void UMMClassSelectWidget::SelectArcher()
{
	if (TempPlayer)
	{
		// 궁수로 전직합니다.
		IMMStatusInterface* StatCharacter = Cast<IMMStatusInterface>(TempPlayer);
		if (StatCharacter)
		{
			StatCharacter->GetStatComponent()->SetClass(EClassType::CT_Archer);
		}

		// 메인 위젯의 창을 스킬 업그레이드 창으로 변경합니다.
		UMMClassTrainerWidget* Widget = Cast<UMMClassTrainerWidget>(MainWidget);
		if (Widget)
		{
			Widget->SwitchClassSkillWidget();
		}
	}
}

void UMMClassSelectWidget::SelectMage()
{
	if (TempPlayer)
	{
		// 마법사로 전직합니다.
		IMMStatusInterface* StatCharacter = Cast<IMMStatusInterface>(TempPlayer);
		if (StatCharacter)
		{
			StatCharacter->GetStatComponent()->SetClass(EClassType::CT_Mage);
		}

		// 메인 위젯의 창을 스킬 업그레이드 창으로 변경합니다.
		UMMClassTrainerWidget* Widget = Cast<UMMClassTrainerWidget>(MainWidget);
		if (Widget)
		{
			Widget->SwitchClassSkillWidget();
		}
	}
}

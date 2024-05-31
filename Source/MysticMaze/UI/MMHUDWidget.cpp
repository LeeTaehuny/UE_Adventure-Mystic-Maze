// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMHUDWidget.h"
#include "UI/MMInventoryWidget.h"
#include "UI/MMInteractionWidget.h"
#include "UI/MMStatusWidget.h"
#include "UI/MMPlayerStatusBarWidget.h"
#include "UI/MMEquipmentWidget.h"
#include "UI/MMSkillWidget.h"
#include "Interface/MMInventoryInterface.h"
#include "Interface/MMStatusInterface.h"
#include "Interface/MMSkillInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMStatComponent.h"
#include "Player/MMSkillComponent.h"

void UMMHUDWidget::Init()
{
	IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(OwningActor);
	IMMStatusInterface* StatusPawn = Cast<IMMStatusInterface>(OwningActor);
	IMMSkillInterface* SkillPawn = Cast<IMMSkillInterface>(OwningActor);


	// 인벤토리 위젯 및 상호작용 위젯 감추기, 이벤트 매핑
	if (InventoryWidget)
	{
		if (InventoryPawn)
		{
			// 인벤토리의 OnChangeInven 델리게이트에 함수 연동
			InventoryPawn->GetInventoryComponent()->OnChangeInven.AddUObject(InventoryWidget, &UMMInventoryWidget::UpdateInventorySlot);
			InventoryPawn->GetInventoryComponent()->OnChangeGold.AddUObject(InventoryWidget, &UMMInventoryWidget::UpdateInventoryGold);
			// 인벤토리 위젯 초기화
			InventoryWidget->SetOwningActor(OwningActor);
			InventoryWidget->Init();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
		
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (PlayerStatusWidget)
	{
		if (StatusPawn)
		{
			// 이벤트 바인딩
			StatusPawn->GetStatComponent()->OnStatChanged.AddUObject(PlayerStatusWidget, &UMMStatusWidget::UpdateStat);
			StatusPawn->GetStatComponent()->OnClassChanged.AddUObject(PlayerStatusWidget, &UMMStatusWidget::UpdateClass);

			// 스테이터스 위젯 초기화
			PlayerStatusWidget->SetOwningActor(OwningActor);
			PlayerStatusWidget->Init();
			PlayerStatusWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (PlayerStatusBarWidget)
	{
		if (InventoryPawn && StatusPawn && SkillPawn)
		{
			// 인벤토리의 OnChangedPotionSlot 델리게이트에 함수 연동
			InventoryPawn->GetInventoryComponent()->OnChangedPotionSlot.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdatePotionSlot);
			
			// 스탯 컴포넌트의 델리게이트에 함수 연동
			StatusPawn->GetStatComponent()->OnHpChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateHpBar);
			StatusPawn->GetStatComponent()->OnMpChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateMpBar);
			StatusPawn->GetStatComponent()->OnExpChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateExpBar);

			// 스킬 컴포넌트의 델리게이트에 함수 연동
			SkillPawn->GetSkillComponent()->OnChangedSkillSlot.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateSkillSlot);
			SkillPawn->GetSkillComponent()->OnCooltimeChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateSkillData);

			// 플레이어 스테이터스바 위젯 초기화
			PlayerStatusBarWidget->SetOwningActor(OwningActor);
			PlayerStatusBarWidget->Init();
		}
	}

	if (EquipmentWidget)
	{
		if (StatusPawn)
		{
			// 스탯컴포넌트의 OnWeaponChanged 델리게이트에 함수 연동
			StatusPawn->GetStatComponent()->OnWeaponChanged.AddUObject(EquipmentWidget, &UMMEquipmentWidget::UpdateStat);

			// 플레이어 장비 위젯 초기화
			EquipmentWidget->SetOwningActor(OwningActor);
			EquipmentWidget->Init();
			EquipmentWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (SkillWidget)
	{
		// TODO : 델리게이트 연동
		SkillPawn->GetSkillComponent()->OnSkillChanged.AddUObject(SkillWidget, &UMMSkillWidget::UpdateSkillSlot);

		// 스킬 위젯 초기화
		SkillWidget->SetOwningActor(OwningActor);
		SkillWidget->Init();
		SkillWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// 모든 비트를 끈 상태로 시작
	VisibilityFlag = static_cast<uint8>(EWidgetFlags::None);
}

void UMMHUDWidget::ToggleInventoryWidget()
{
	if (!InventoryWidget) return;

	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		// INVENTORY 위젯을 껐다고 표시합니다.
		VisibilityFlag &= ~static_cast<uint8>(EWidgetFlags::INVENTORY);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		// INVENTORY 위젯을 켰다고 표시합니다.
		VisibilityFlag |= static_cast<uint8>(EWidgetFlags::INVENTORY);
	}
}

void UMMHUDWidget::ToggleStatusWidget()
{
	if (!PlayerStatusWidget) return;

	if (PlayerStatusWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		PlayerStatusWidget->SetVisibility(ESlateVisibility::Hidden);

		// STATUS 위젯을 껐다고 표시합니다.
		VisibilityFlag &= ~static_cast<uint8>(EWidgetFlags::STATUS);
	}
	else
	{
		PlayerStatusWidget->SetVisibility(ESlateVisibility::Visible);

		// STATUS 위젯을 켰다고 표시합니다.
		VisibilityFlag |= static_cast<uint8>(EWidgetFlags::STATUS);
	}
}

void UMMHUDWidget::ToggleEquipmentWidget()
{
	if (!EquipmentWidget) return;

	if (EquipmentWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		EquipmentWidget->SetVisibility(ESlateVisibility::Hidden);

		// EQUIPMENT 위젯을 껐다고 표시합니다.
		VisibilityFlag &= ~static_cast<uint8>(EWidgetFlags::EQUIPMENT);
	}
	else
	{
		EquipmentWidget->SetVisibility(ESlateVisibility::Visible);

		// EQUIPMENT 위젯을 켰다고 표시합니다.
		VisibilityFlag |= static_cast<uint8>(EWidgetFlags::EQUIPMENT);
	}
}

void UMMHUDWidget::ToggleSkillWidget()
{
	if (!SkillWidget) return;

	if (SkillWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		SkillWidget->SetVisibility(ESlateVisibility::Hidden);

		// SKILL 위젯을 껐다고 표시합니다.
		VisibilityFlag &= ~static_cast<uint8>(EWidgetFlags::SKILL);
	}
	else
	{
		SkillWidget->SetVisibility(ESlateVisibility::Visible);

		// SKILL 위젯을 켰다고 표시합니다.
		VisibilityFlag |= static_cast<uint8>(EWidgetFlags::SKILL);
	}
}

void UMMHUDWidget::ToggleInteractionWidget(bool InValue)
{
	if (!InteractionWidget) return;

	if (InValue)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMMHUDWidget::InteractionWidgetHelpText(FString HelpText)
{
	if (!InteractionWidget) return;

	InteractionWidget->SetHelpText(HelpText);
}

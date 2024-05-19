// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMHUDWidget.h"
#include "UI/MMInventoryWidget.h"
#include "UI/MMInteractionWidget.h"
#include "UI/MMPlayerStatusBarWidget.h"
#include "Interface/MMInventoryInterface.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMStatComponent.h"

void UMMHUDWidget::Init()
{
	IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(OwningActor);
	IMMStatusInterface* StatusPawn = Cast<IMMStatusInterface>(OwningActor);

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
	
	if (PlayerStatusBarWidget)
	{
		if (InventoryPawn && StatusPawn)
		{
			// 인벤토리의 OnChangedPotionSlot 델리게이트에 함수 연동
			InventoryPawn->GetInventoryComponent()->OnChangedPotionSlot.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdatePotionSlot);
			
			// 스탯 컴포넌트의 델리게이트에 함수 연동
			StatusPawn->GetStatComponent()->OnHpChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateHpBar);
			StatusPawn->GetStatComponent()->OnMpChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateMpBar);
			StatusPawn->GetStatComponent()->OnExpChanged.AddUObject(PlayerStatusBarWidget, &UMMPlayerStatusBarWidget::UpdateExpBar);

			// 플레이어 스테이터스바 위젯 초기화
			PlayerStatusBarWidget->SetOwningActor(OwningActor);
			PlayerStatusBarWidget->Init();
		}
	}
}

bool UMMHUDWidget::ToggleInventoryWidget()
{
	if (!InventoryWidget) return false;

	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		return false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		return true;
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMHUDWidget.h"
#include "UI/MMInventoryWidget.h"
#include "UI/MMInteractionWidget.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"

void UMMHUDWidget::Init()
{
	// 인벤토리 위젯 및 상호작용 위젯 감추기, 이벤트 매핑
	if (InventoryWidget)
	{
		IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(OwningActor);
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

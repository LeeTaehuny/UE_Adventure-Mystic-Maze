// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMPlayerController.h"
#include "Interface/MMInventoryInterface.h"
#include "UI/MMInventoryWidget.h"
#include "Player/MMInventoryComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AMMPlayerController::AMMPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMMInventoryWidget>InventoryClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_Inventory.WBP_Inventory_C'"));
	if (InventoryClassRef.Succeeded())
	{
		InventoryWidgetClass = InventoryClassRef.Class;
	}
}

void AMMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	InitInventoryWidget();
}

void AMMPlayerController::ToggleInventoryVisibility()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		SetGameInputMode();
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInputMode();
	}
}

void AMMPlayerController::InitInventoryWidget()
{
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UMMInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(GetPawn());
			if (InventoryPawn)
			{
				// 인벤토리의 OnChangeInven 델리게이트에 함수 연동
				InventoryPawn->GetInventoryComponent()->OnChangeInven.AddUObject(InventoryWidget, &UMMInventoryWidget::UpdateInventorySlot);
				InventoryPawn->GetInventoryComponent()->OnChangeGold.AddUObject(InventoryWidget, &UMMInventoryWidget::UpdateInventoryGold);
				// 인벤토리 위젯 초기화
				InventoryWidget->SetOwningActor(GetPawn());
				InventoryWidget->AddToViewport();
				InventoryWidget->Init();
				InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AMMPlayerController::SetUIInputMode()
{
	// TODO : 더 세밀하게 변경되도록 설정 고치기
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
	bShowMouseCursor = true;
}

void AMMPlayerController::SetGameInputMode()
{
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	bShowMouseCursor = false;
}


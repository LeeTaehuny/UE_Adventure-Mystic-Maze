// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMPlayerController.h"
#include "UI/MMInventoryWidget.h"
#include "Player/MMInventoryComponent.h"
#include "UI/MMHUDWidget.h"

AMMPlayerController::AMMPlayerController()
{
	// HUD Widget Class 찾기
	static ConstructorHelpers::FClassFinder<UMMHUDWidget>HUDWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_HUD.WBP_HUD_C'"));
	if (HUDWidgetClassRef.Succeeded())
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
}

void AMMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitHUDWidget();
}

void AMMPlayerController::InitHUDWidget()
{
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UMMHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->SetOwningActor(GetPawn());
			HUDWidget->Init();
			HUDWidget->AddToViewport();
		}
	}
}

void AMMPlayerController::ToggleInventoryVisibility()
{
	if (HUDWidget)
	{
		if (HUDWidget->ToggleInventoryWidget())
		{
			// 인벤토리 위젯이 보이는 경우 UI모드로 변경합니다.
			SetUIInputMode();
		}
		else
		{
			// 인벤토리 위젯이 안보이는 경우 Game모드로 변경합니다.
			SetGameInputMode();
		}
	}
}

void AMMPlayerController::ToggleInteractionVisibility(bool InValue)
{
	if (HUDWidget)
	{
		HUDWidget->ToggleInteractionWidget(InValue);
	}
}

void AMMPlayerController::InteractionWidgetHelpText(FString HelpText)
{
	if (HUDWidget)
	{
		HUDWidget->InteractionWidgetHelpText(HelpText);
	}
}

void AMMPlayerController::SetUIInputMode()
{
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
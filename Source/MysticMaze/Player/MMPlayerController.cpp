// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMPlayerController.h"
#include "UI/MMInventoryWidget.h"
#include "Player/MMInventoryComponent.h"
#include "UI/MMHUDWidget.h"
#include "UI/MMSettingWidget.h"

AMMPlayerController::AMMPlayerController()
{
	// HUD Widget Class 찾기
	static ConstructorHelpers::FClassFinder<UMMHUDWidget>HUDWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_HUD.WBP_HUD_C'"));
	if (HUDWidgetClassRef.Succeeded())
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}

	// Setting Widget Class 찾기
	static ConstructorHelpers::FClassFinder<UMMSettingWidget>SettingWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_Setting.WBP_Setting_C'"));
	if (SettingWidgetClassRef.Succeeded())
	{
		SettingWidgetClass = SettingWidgetClassRef.Class;
	}
}

void AMMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitHUDWidget();

	SettingWidget = CreateWidget<UMMSettingWidget>(GetWorld(), SettingWidgetClass);
	if (SettingWidget)
	{
		SettingWidget->SetOwningActor(GetPawn());
		SettingWidget->AddToViewport();
		SettingWidget->SetVisibility(ESlateVisibility::Hidden);
	}
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
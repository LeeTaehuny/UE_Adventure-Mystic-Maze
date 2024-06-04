// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartLevel/MMStartWidget.h"
#include "UI/StartLevel/MMMainStartWidget.h"
#include "Game/MMGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMMStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 연동
	if (BTN_StartGame)
		BTN_StartGame->OnClicked.AddDynamic(this, &UMMStartWidget::StartGame);

	if (BTN_Exit)
		BTN_Exit->OnClicked.AddDynamic(this, &UMMStartWidget::ExitGame);
}

void UMMStartWidget::Init(UUserWidget* MainWidget)
{
	if (!MainWidget) return;
	Main = MainWidget;
}

void UMMStartWidget::StartGame()
{
	// SelectDataWidget으로 전환
	if (UMMMainStartWidget* MainWidget = Cast<UMMMainStartWidget>(Main))
	{
		MainWidget->GoToSelect();
	}
}

void UMMStartWidget::ExitGame()
{
	// 게임 종료하기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->Exit();
	}
}

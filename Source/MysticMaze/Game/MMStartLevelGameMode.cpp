// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMStartLevelGameMode.h"
#include "UI/StartLevel/MMMainStartWidget.h"

#include "Kismet/GameplayStatics.h"

AMMStartLevelGameMode::AMMStartLevelGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/StartLevel/WBP_StartMain.WBP_StartMain_C'"));
	if (MenuBPClass.Class != NULL)
	{
		MenuClass = MenuBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_Loading.WBP_Loading_C'"));
	if (LoadingBPClass.Class != NULL)
	{
		LoadingWidgetClass = LoadingBPClass.Class;
	}
}

void AMMStartLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!MenuClass) return;

	// 위젯 생성 및 출력
	Menu = CreateWidget<UMMMainStartWidget>(GetWorld(), MenuClass);
	Menu->AddToViewport();

	// 메뉴 위젯 초기화
	Menu->Init();
}

void AMMStartLevelGameMode::ChangeMainLevel()
{
	if (Menu)
		Menu->RemoveFromViewport();

	UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport();

		LoadingTimerHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, this, &AMMStartLevelGameMode::OpenLevel, 1.0f, false);
	}
}

void AMMStartLevelGameMode::OpenLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainLevelName);
}

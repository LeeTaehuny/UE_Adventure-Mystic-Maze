// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMStartLevelGameMode.h"
#include "UI/StartLevel/MMMainStartWidget.h"

#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->OnAudioFinished.AddDynamic(this, &AMMStartLevelGameMode::OnSoundFinished);

	static ConstructorHelpers::FObjectFinder<USoundWave> SoundWaveAsset(TEXT("/Script/Engine.SoundWave'/Game/MysticMaze/Game/LobbyBGM.LobbyBGM'"));
	if (SoundWaveAsset.Succeeded())
	{
		AudioComponent->SetSound(SoundWaveAsset.Object);
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

	if (AudioComponent && AudioComponent->GetSound())
	{
		AudioComponent->Play();
	}
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

void AMMStartLevelGameMode::OnSoundFinished()
{
	if (AudioComponent && AudioComponent->GetSound())
	{
		AudioComponent->Play();
	}
}

void AMMStartLevelGameMode::OpenLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainLevelName);
}

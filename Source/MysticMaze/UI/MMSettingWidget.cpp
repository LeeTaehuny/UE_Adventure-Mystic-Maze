// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMSettingWidget.h"
#include "Interface/MMSaveInterface.h"
#include "Game/MMGameInstance.h"
#include "Player/MMPlayerController.h"

#include "GameFramework/Character.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMMSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Resume)
		BTN_Resume->OnClicked.AddDynamic(this, &UMMSettingWidget::ResumeGame);

	if (BTN_SaveExit)
		BTN_SaveExit->OnClicked.AddDynamic(this, &UMMSettingWidget::ExitGame);
}

void UMMSettingWidget::ResumeGame()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OwningActor);
	if (PlayerCharacter)
	{
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			// 게임을 재개합니다.
			SetVisibility(ESlateVisibility::Hidden);
			// 게임모드로 변환합니다.
			PlayerController->SetGameInputMode();
		}
	}
}

void UMMSettingWidget::ExitGame()
{
	IMMSaveInterface* SavePlayer = Cast<IMMSaveInterface>(OwningActor);
	if (SavePlayer)
	{
		SavePlayer->Save();
	}

	// 게임 종료하기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->Exit();
	}
}

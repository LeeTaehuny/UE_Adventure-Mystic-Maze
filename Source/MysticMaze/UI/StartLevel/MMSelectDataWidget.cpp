// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartLevel/MMSelectDataWidget.h"
#include "UI/StartLevel/MMMainStartWidget.h"
#include "Game/MMStartLevelGameMode.h"
#include "Game/MMGameInstance.h"
#include "Game/MMSaveGameData.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMMSelectDataWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 연동
	if (BTN_Slot1)
		BTN_Slot1->OnClicked.AddDynamic(this, &UMMSelectDataWidget::SelectSaveSlot1);

	if (BTN_Slot2)
		BTN_Slot2->OnClicked.AddDynamic(this, &UMMSelectDataWidget::SelectSaveSlot2);

	if (BTN_Slot3)
		BTN_Slot3->OnClicked.AddDynamic(this, &UMMSelectDataWidget::SelectSaveSlot3);

	if (BTN_Back)
		BTN_Back->OnClicked.AddDynamic(this, &UMMSelectDataWidget::BackMain);

	SaveSlotText.Add(TXT_SaveData1);
	SaveSlotText.Add(TXT_SaveData2);
	SaveSlotText.Add(TXT_SaveData3);
}

void UMMSelectDataWidget::Init(UUserWidget* MainWidget)
{
	if (!MainWidget) return;
	Main = MainWidget;

	UpdateText();
}

void UMMSelectDataWidget::SelectSaveSlot1()
{
	// 세이브 파일 이름 지정
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetSaveSlot(1);
	}

	// 게임 시작
	GameStart();
}

void UMMSelectDataWidget::SelectSaveSlot2()
{
	// 세이브 파일 이름 지정
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetSaveSlot(2);
	}

	// 게임 시작
	GameStart();
}

void UMMSelectDataWidget::SelectSaveSlot3()
{
	// 세이브 파일 이름 지정
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetSaveSlot(3);
	}

	// 게임 시작
	GameStart();
}

void UMMSelectDataWidget::UpdateText()
{
	// GameInstance에서 Save파일 이름 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		FString SaveFileName = GameInstance->GetSlotName();

		// 데이터 로드 
		for (int i = 1; i < 4; i++)
		{
			UMMSaveGameData* GameData = Cast<UMMSaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveFileName + FString::FromInt(i), 0));
			if (GameData)
			{
				FString ClassName;
				switch (GameData->Class)
				{
				case EClassType::CT_Beginner:
					ClassName = TEXT("Beginner");
					break;

				case EClassType::CT_Warrior:
					ClassName = TEXT("Warrior");
					break;

				case EClassType::CT_Archer:
					ClassName = TEXT("Archer");
					break;

				case EClassType::CT_Mage:
					ClassName = TEXT("Mage");
					break;
				}

				// 데이터가 있는 경우 레벨, 직업, 현재 층 수를 출력합니다.
				SaveSlotText[i - 1]->SetText(FText::FromString(FString::Printf(TEXT("Lv: %d, Class: %s, ClearFloor : %d"), GameData->Level, *ClassName, GameData->CurrentFloor)));
			}
			else
			{
				// 데이터가 없는 경우 비어있다고 표시합니다.
				SaveSlotText[i - 1]->SetText(FText::FromString(TEXT("Empty")));
			}
		}
	}
}

void UMMSelectDataWidget::GameStart()
{
	// 게임 모드로 전환
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (!PlayerController) return;

		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);

		PlayerController->bShowMouseCursor = false;
	}

	// GameMode에서 레벨 전환하기
	AMMStartLevelGameMode* GameMode = Cast<AMMStartLevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->ChangeMainLevel();
	}
}

void UMMSelectDataWidget::BackMain()
{
	// StartWidget으로 전환
	if (UMMMainStartWidget* MainWidget = Cast<UMMMainStartWidget>(Main))
	{
		MainWidget->BackToMain();
	}
}

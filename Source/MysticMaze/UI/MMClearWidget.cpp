// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMClearWidget.h"
#include "Game/MMGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMMClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Main)
		BTN_Main->OnClicked.AddDynamic(this, &UMMClearWidget::GoMain);

	// 컨트롤러 설정 (UI모드로 전환)
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		// 플레이어 컨트롤러 받아오기
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (!PlayerController) return;

		FInputModeUIOnly InputModeData;
		// 마우스를 뷰포트에 잠그지 않도록 지정
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		// 위에서 만든 Data를 PlayerController에 적용하기
		PlayerController->SetInputMode(InputModeData);

		// 마우스 커서가 보이도록 설정
		PlayerController->bShowMouseCursor = true;
	}
}

void UMMClearWidget::GoMain()
{
	// 세이브 파일 이름 초기화
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->InitSaveFileName();
	}

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
}

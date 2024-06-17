// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartLevel/MMMainStartWidget.h"
#include "UI/StartLevel/MMStartWidget.h"
#include "UI/StartLevel/MMSelectDataWidget.h"

#include "Components/WidgetSwitcher.h"

void UMMMainStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMMainStartWidget::Init()
{
	if (!StartWidget || !SelectDataWidget) return;

	StartWidget->Init(this);
	SelectDataWidget->Init(this);

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

	WidgetSwitcher->SetActiveWidget(StartWidget);
}

void UMMMainStartWidget::BackToMain()
{
	WidgetSwitcher->SetActiveWidget(StartWidget);
}

void UMMMainStartWidget::GoToSelect()
{
	WidgetSwitcher->SetActiveWidget(SelectDataWidget);
}

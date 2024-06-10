// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMClearWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMMClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Main)
		BTN_Main->OnClicked.AddDynamic(this, &UMMClearWidget::GoMain);
}

void UMMClearWidget::GoMain()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
}

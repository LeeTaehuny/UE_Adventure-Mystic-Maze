// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMInteractionWidget.h"
#include "Components/TextBlock.h"

void UMMInteractionWidget::SetHelpText(FString HelpText)
{
	TXT_HelpText->SetText(FText::FromString(HelpText));
}

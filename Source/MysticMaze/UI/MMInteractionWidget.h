// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMInteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_HelpText;

public:
	void SetHelpText(FString HelpText);
};

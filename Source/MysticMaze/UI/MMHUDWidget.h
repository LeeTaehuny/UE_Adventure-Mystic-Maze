// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "MMHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMHUDWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMInventoryWidget> InventoryWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMInteractionWidget> InteractionWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMPlayerStatusBarWidget> PlayerStatusBarWidget;

public:
	void Init();

	bool ToggleInventoryWidget();
	void ToggleInteractionWidget(bool InValue);
	void InteractionWidgetHelpText(FString HelpText);
};

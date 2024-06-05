// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMMainStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMMainStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void BackToMain();
	void GoToSelect();

public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMStartWidget> StartWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMSelectDataWidget> SelectDataWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "Interface/MMWidgetChangeInterface.h"
#include "MMClassSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMClassSelectWidget : public UMMCustomWidget, public IMMWidgetChangeInterface
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init(AActor* InPlayer, UUserWidget* InMainWidget);
	virtual void UpdateWidget() override;

public:
	UFUNCTION()
	void SelectWarrior();

	UFUNCTION()
	void SelectArcher();

	UFUNCTION()
	void SelectMage();

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_SelectWarrior;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_SelectArcher;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_SelectMage;

private:
	UPROPERTY()
	TObjectPtr<AActor> TempPlayer;

	UPROPERTY()
	TObjectPtr<UUserWidget> MainWidget;
};

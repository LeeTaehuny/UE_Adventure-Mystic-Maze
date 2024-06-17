// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMClassTrainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMClassTrainerWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void Init(AActor* InPlayer);
	void SwitchClassSkillWidget();
	void UpdateWidget();
	void UpdateGold();

public:
	UFUNCTION()
	void Close();

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Close;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMClassSelectWidget> ClassSelectWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMClassSkillWidget> ClassSkillWidget;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CurrentGold;

private:
	void ChangeWidget(UUserWidget* NewWidget);

	// 플레이어 저장용 변수
	UPROPERTY()
	TObjectPtr<AActor> TempPlayer;

	UPROPERTY()
	TObjectPtr<UUserWidget> TempWidget;
};

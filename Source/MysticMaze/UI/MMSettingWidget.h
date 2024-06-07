// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "MMSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSettingWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void ExitGame();

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Resume;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_SaveExit;
};

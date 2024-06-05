// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init(UUserWidget* MainWidget);

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void ExitGame();

public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_StartGame;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Exit;

private:
	UPROPERTY()
	TObjectPtr<class UUserWidget> Main;
};

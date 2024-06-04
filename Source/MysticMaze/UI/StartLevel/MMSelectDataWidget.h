// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMSelectDataWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSelectDataWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init(UUserWidget* MainWidget);
	
	UFUNCTION()
	void SelectSaveSlot1();

	UFUNCTION()
	void SelectSaveSlot2();

	UFUNCTION()
	void SelectSaveSlot3();

	UFUNCTION()
	void BackMain();

public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Slot1;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Slot2;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Slot3;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_SaveData1;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_SaveData2;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_SaveData3;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_Back;


private:
	void UpdateText();
	void GameStart();

	UPROPERTY()
	TObjectPtr<class UUserWidget> Main;

	TArray<TObjectPtr<class UTextBlock>> SaveSlotText;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "Interface/MMWidgetChangeInterface.h"
#include "MMClassSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMClassSkillWidget : public UMMCustomWidget, public IMMWidgetChangeInterface
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void Init(AActor* InPlayer, EClassType InPlayerType);
	virtual void UpdateWidget() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UMMSlot>> SkillSlots;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UTextBlock>> SkillLevels;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UTextBlock>> SkillNames;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UMultiLineEditableTextBox>> SkillDescs;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UTextBlock>> SkillCoolTimes;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UTextBlock>> SkillManaCosts;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class UMMCustomButton>> SkillUpgradeButtons;

private:
	// 플레이어 저장용 변수
	UPROPERTY()
	TObjectPtr<AActor> TempPlayer;

	ESlotType SkillType;
};

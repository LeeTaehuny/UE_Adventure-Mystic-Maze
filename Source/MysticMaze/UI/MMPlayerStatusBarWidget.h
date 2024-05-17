// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMPlayerStatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMPlayerStatusBarWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void UpdateSkillSlot();
	void UpdatePotionSlot();

// HpBar
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UProgressBar> PB_HpBar;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CurrentHp;
	
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MaxHp;
	
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_HpPercent;

// MpBar
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UProgressBar> PB_MpBar;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CurrentMp;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MaxMp;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MpPercent;

// ExpBar
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UProgressBar> PB_ExpBar;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_ExpPercent;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CurrentExp;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MaxExp;

// Quick Slot (Skill, Potion)
public:
	UPROPERTY(VisibleAnywhere, Category = "SkillSlot")
	TArray<TObjectPtr<class UMMSlot>> SkillSlots;

	UPROPERTY(VisibleAnywhere, Category = "SkillSlot")
	TArray<TObjectPtr<class UTextBlock>> CoolTimeTexts;

	UPROPERTY(VisibleAnywhere, Category = "SkillSlot")
	TArray<TObjectPtr<class UProgressBar>> CoolTimeProgressBars;
	
	UPROPERTY(VisibleAnywhere, Category = "PotionSlot")
	TArray<TObjectPtr<class UMMSlot>> PotionSlots;
};

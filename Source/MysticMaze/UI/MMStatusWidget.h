// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMCharacterStat.h"
#include "MMStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMStatusWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void Init();
	void UpdateStat(const FMMCharacterStat& InTotalStat);
	void UpdateClass();

private:
	UFUNCTION()
	void MoveStart();

	UFUNCTION()
	void MoveEnd();

	UFUNCTION()
	void UpgradeSTR();

	UFUNCTION()
	void UpgradeDEX();

	UFUNCTION()
	void UpgradeCON();

	UFUNCTION()
	void UpgradeINT();

// Main
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_MainButton;

// Basic Stat
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_TotalSTR;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_UpgradeSTR;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_TotalDEX;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_UpgradeDEX;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_TotalCON;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_UpgradeCON;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_TotalINT;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_UpgradeINT;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_Class;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CurrentLevel;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_StatPoint;

// Detail Stat
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MaxHp;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MaxMp;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_AttackDamage;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_Defense;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_MovementSpeed;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_AttackSpeed;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_CriticalHitRate;

private:
	uint8 bIsDragging;

	FVector2D InitialOffset;
	FVector2D InitialPos;

	TObjectPtr<class UBorder> Border;
};

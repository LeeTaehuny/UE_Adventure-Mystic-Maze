// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMCharacterStat.h"
#include "MMEquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMEquipmentWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void UpdateStat(const FMMCharacterStat& InWeaponStat);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void MoveStart();

	UFUNCTION()
	void MoveEnd();

public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_MainButton;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMMSlot> EquipmentSlot;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_WeaponSTR;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_WeaponDEX;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_WeaponCON;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_WeaponINT;
	
private:
	uint8 bIsDragging;

	FVector2D InitialOffset;
	FVector2D InitialPos;

	TObjectPtr<class UBorder> Border;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMCustomStat.generated.h"

DECLARE_DELEGATE(FOnUpdateStatDelegate);
USTRUCT(BlueprintType)
struct FUpdateStatDelegateWrapper
{
	GENERATED_BODY()

	FUpdateStatDelegateWrapper() { }
	FUpdateStatDelegateWrapper(const FOnUpdateStatDelegate& InStatDelegate) : StatDelegate(InStatDelegate) { }

	FOnUpdateStatDelegate StatDelegate;
};

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMCustomStat : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void SetType(EClassType Type);
	void UpdateStat();

public:
	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_StatSTR;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_StatDEX;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_StatCON;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_StatINT;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	EClassType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 WeaponIndex;

protected:
	UPROPERTY()
	TMap<EClassType, FUpdateStatDelegateWrapper> StatUpdateActions;

	void WarriorWeapon();
	void ArcherWeapon();
	void MageWeapon();
};

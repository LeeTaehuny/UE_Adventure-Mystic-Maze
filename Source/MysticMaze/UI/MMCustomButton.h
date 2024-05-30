// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "GameData/MMEnums.h"
#include "MMCustomButton.generated.h"

DECLARE_DELEGATE(FOnUpdateButtonDelegate);
USTRUCT(BlueprintType)
struct FUpdateButtonDelegateWrapper
{
	GENERATED_BODY()

	FUpdateButtonDelegateWrapper() { }
	FUpdateButtonDelegateWrapper(const FOnUpdateButtonDelegate& InButtonDelegate) : ButtonDelegate(InButtonDelegate) { }

	FOnUpdateButtonDelegate ButtonDelegate;
};

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMCustomButton : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init();
	void SetType(ESlotType Type);
	void UpdateText();

public:
	UFUNCTION()
	void Purchase();
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Button")
	ESlotType ButtonType;

	UPROPERTY(VisibleAnywhere, Category = "Button", meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_CustomButton;

	UPROPERTY(EditAnywhere, Category = "Button")
	int32 ButtonIndex;

	UPROPERTY(VisibleAnywhere, Category = "Button", meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_Price;

protected:
	UPROPERTY()
	TMap<ESlotType, FUpdateButtonDelegateWrapper> TextUpdateActions;

	void UpdatePotionShopButton();
	void UpdateWeaponWarriorShopButton();
	void UpdateWeaponArcherShopButton();
	void UpdateWeaponMageShopButton();

	void PurchaseItem(int32 Index);
	void PurchaseWeapon(int32 Index);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "MMSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkillWidget : public UMMCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void Init();
	void UpdateSkillSlot();

private:
	UFUNCTION()
	void MoveStart();

	UFUNCTION()
	void MoveEnd();

// Main
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UButton> BTN_MainButton;

private:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UMMSlot>> Slots;

	uint8 bIsDragging;

	FVector2D InitialOffset;
	FVector2D InitialPos;

	TObjectPtr<class UBorder> Border;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "MMMonsterHPBar.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMMonsterHPBar : public UMMCustomWidget
{
	GENERATED_BODY()

public:

	void Init();
	void UpdateHPBar(float CurHP, float MaxHP);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UProgressBar> MonsterHPBar;
	
};

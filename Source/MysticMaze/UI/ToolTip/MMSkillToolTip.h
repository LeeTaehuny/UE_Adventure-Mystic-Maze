// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ToolTip/MMToolTip.h"
#include "MMSkillToolTip.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkillToolTip : public UMMToolTip
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_RequireLevel;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_SkillLevel;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UMultiLineEditableTextBox> TXTB_SkillDesc;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_SkillCoolTime;

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_ManaCost;
};

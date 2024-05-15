// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ToolTip/MMToolTip.h"
#include "MMOtherToolTip.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMOtherToolTip : public UMMToolTip
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_ItemType;
};

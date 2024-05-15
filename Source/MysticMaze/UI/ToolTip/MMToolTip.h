// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MMCustomWidget.h"
#include "MMToolTip.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMToolTip : public UMMCustomWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_ItemName;
};

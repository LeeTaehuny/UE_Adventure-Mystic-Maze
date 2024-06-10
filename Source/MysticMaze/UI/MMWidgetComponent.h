// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MMWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	virtual void InitWidget() override;;
	
};

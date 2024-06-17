// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_Dungeon_UI.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_Dungeon_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void Init();
	void UpdateText();

	UPROPERTY(meta = (BindWidget = "true"))
	TObjectPtr<class UTextBlock> TXT_Number;
};

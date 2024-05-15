// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MMPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMMPlayerController();

protected:
	void BeginPlay() override;

public:
	void ToggleInventoryVisibility();
	void ToggleInteractionVisibility(bool InValue);
	void InteractionWidgetHelpText(FString HelpText);
	
// Widget Section
protected:
	void SetUIInputMode();
	void SetGameInputMode();

	void InitHUDWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UMMHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UMMHUDWidget> HUDWidget;
};

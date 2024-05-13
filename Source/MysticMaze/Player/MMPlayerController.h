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
	

// Widget Section
protected:
	void InitInventoryWidget();
	void SetUIInputMode();
	void SetGameInputMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UMMInventoryWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UMMInventoryWidget> InventoryWidget;
};

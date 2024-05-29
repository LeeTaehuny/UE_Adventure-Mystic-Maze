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
	FORCEINLINE class UMMHUDWidget* GetHUDWidget() { return HUDWidget; }

	void SetUIInputMode();
	void SetGameInputMode();

	
// Widget Section
protected:
	void InitHUDWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UMMHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UMMHUDWidget> HUDWidget;
};

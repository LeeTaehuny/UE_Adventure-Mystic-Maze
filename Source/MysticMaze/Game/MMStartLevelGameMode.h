// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MMStartLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMStartLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMMStartLevelGameMode();

protected:
	virtual void BeginPlay() override;

public:
	void ChangeMainLevel();

private:
	UFUNCTION()
	void OnSoundFinished();

	void OpenLevel();

	UPROPERTY(EditAnywhere, Category = "Level", meta = (PrivateAllowAccess = "true"))
	FName MainLevelName;

	TSubclassOf<class UUserWidget> MenuClass;

	UPROPERTY(VisibleAnywhere, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMMMainStartWidget> Menu;

	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	FTimerHandle LoadingTimerHandle;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;
};

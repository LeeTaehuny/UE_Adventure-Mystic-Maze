// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MMGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMMGameMode();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnSoundFinished();

	TSubclassOf<class AMMMonsterFieldSpawner> SpawnerData;
	TObjectPtr<class AMMMonsterFieldSpawner> Spawner;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;
};

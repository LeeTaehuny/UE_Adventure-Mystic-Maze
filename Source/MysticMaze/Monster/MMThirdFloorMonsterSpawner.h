// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMThirdFloorMonsterSpawner.generated.h"

UCLASS()
class MYSTICMAZE_API AMMThirdFloorMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMThirdFloorMonsterSpawner();

	void SetSpawnPoint(FVector INPoint) { SpawnPoint.Add(INPoint); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	TArray<FVector> SpawnPoint;
};

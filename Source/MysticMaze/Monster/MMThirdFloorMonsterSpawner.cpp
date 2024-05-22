// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMThirdFloorMonsterSpawner.h"

// Sets default values
AMMThirdFloorMonsterSpawner::AMMThirdFloorMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("ThirdFloorSpawner");
}

// Called when the game starts or when spawned
void AMMThirdFloorMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMMThirdFloorMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


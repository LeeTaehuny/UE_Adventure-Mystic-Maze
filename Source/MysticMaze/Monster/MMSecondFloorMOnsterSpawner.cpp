// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMSecondFloorMOnsterSpawner.h"

// Sets default values
AMMSecondFloorMOnsterSpawner::AMMSecondFloorMOnsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("SecondFloorSpawner");
}

// Called when the game starts or when spawned
void AMMSecondFloorMOnsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMMSecondFloorMOnsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


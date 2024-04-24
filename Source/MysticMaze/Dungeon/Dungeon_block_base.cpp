// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/Dungeon_block_base.h"

// Sets default values
ADungeon_block_base::ADungeon_block_base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeon_block_base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeon_block_base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


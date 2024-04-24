// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"

/**
 * 
 */
class MYSTICMAZE_API First_Dungeon_Manager : public AActor
{
public:
	First_Dungeon_Manager();
	~First_Dungeon_Manager();

	UPROPERTY(BlueprintCallable)
	void BeginPlay();

private:

	TArray<class Dungeon_block_base*> block;
};
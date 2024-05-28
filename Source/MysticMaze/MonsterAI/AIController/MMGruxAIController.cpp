// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMGruxAIController.h"
#include "Interface/MMAnimationBugSwarmAIInterface.h"

AMMGruxAIController::AMMGruxAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMMGruxAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMMGruxAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

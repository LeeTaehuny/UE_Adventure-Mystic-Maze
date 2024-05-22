// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMMechaniteAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

AMMMechaniteAIController::AMMMechaniteAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMMMechaniteAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BegAIBehavior)
	{
		RunBehaviorTree(BegAIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Blackboard->SetValueAsObject("TargetPlayer", PlayerPawn);
		}
	}
}

void AMMMechaniteAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

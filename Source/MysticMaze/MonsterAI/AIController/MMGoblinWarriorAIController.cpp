// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMGoblinWarriorAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AMMGoblinWarriorAIController::AMMGoblinWarriorAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMMGoblinWarriorAIController::BeginPlay()
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

void AMMGoblinWarriorAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMGruxAIController.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AMMGruxAIController::AMMGruxAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMMGruxAIController::BeginPlay()
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

void AMMGruxAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IMMBugSwarmAIDataInterface* Monster = Cast<IMMBugSwarmAIDataInterface>(GetPawn());
	if (Monster)
	{
		Blackboard->SetValueAsBool("Die", Monster->GetDie());
		Blackboard->SetValueAsBool("Spawn", Monster->GetSpawn());
		Blackboard->SetValueAsBool("ATK_Mode", Monster->GetATKMode());
	}
}

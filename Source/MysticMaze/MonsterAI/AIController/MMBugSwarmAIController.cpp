// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMBugSwarmAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "Kismet/GameplayStatics.h"

AMMBugSwarmAIController::AMMBugSwarmAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	bLeaderItMe = false;
	bRehabilitation = false;
	bSpeedAttack = false;
	bMachineGun = false;
}

void AMMBugSwarmAIController::SettingBlackBoardData(bool INDie, bool INSpawn)
{
	if (BegAIBehavior)
	{
		Blackboard->SetValueAsBool("Die", INDie);
		Blackboard->SetValueAsBool("Spawn", INSpawn);
	}
}

void AMMBugSwarmAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BegAIBehavior)
	{
		RunBehaviorTree(BegAIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Blackboard->SetValueAsObject("TargetPlayer", PlayerPawn);
			Blackboard->SetValueAsBool("bRehabilitation", true);
			Blackboard->SetValueAsBool("bSpeedAttack", false);
			Blackboard->SetValueAsBool("bMachineGun", false);
		}
	}
}

void AMMBugSwarmAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IMMBugSwarmAIDataInterface* BugSwarm = Cast<IMMBugSwarmAIDataInterface>(GetPawn());
	if (BugSwarm)
	{
		Blackboard->SetValueAsBool("Die", BugSwarm->GetDie());
		Blackboard->SetValueAsBool("Spawn", BugSwarm->GetSpawn());
		Blackboard->SetValueAsBool("ATK_Mode", BugSwarm->GetATKMode());
	}
}

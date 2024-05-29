// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MM_BugSwarm_ATKChoice_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAI/AIController/MMBugSwarmAIController.h"

UMM_BugSwarm_ATKChoice_Task::UMM_BugSwarm_ATKChoice_Task()
{
}

EBTNodeResult::Type UMM_BugSwarm_ATKChoice_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	AMMBugSwarmAIController* MyAiController = Cast<AMMBugSwarmAIController>(OwnerComp.GetAIOwner());
	int RandomATK = FMath::RandRange(0, 2);
	RandomATK = 0;
	switch (RandomATK)
	{
	case 0:
		MyAiController->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", true);
		MyAiController->GetBlackboardComponent()->SetValueAsBool("bSpeedAttack", true);
		break;

	case 1:
		MyAiController->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", true);
		MyAiController->GetBlackboardComponent()->SetValueAsBool("bMachineGun", true);
		break;

	case 2:
		break;
	}

	return EBTNodeResult::Failed;
}

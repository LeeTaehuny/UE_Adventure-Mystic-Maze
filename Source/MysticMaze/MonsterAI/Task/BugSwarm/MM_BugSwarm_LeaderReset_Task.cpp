// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MM_BugSwarm_LeaderReset_Task.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMM_BugSwarm_LeaderReset_Task::UMM_BugSwarm_LeaderReset_Task()
{
}

EBTNodeResult::Type UMM_BugSwarm_LeaderReset_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsObject("MyLeader", nullptr);
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("LeaderItMe", false);
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", false);
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bSpeedAttack", false);
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bMachineGun", false);
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector("RehabilitationLocation", FVector());

	return EBTNodeResult::Failed;
}

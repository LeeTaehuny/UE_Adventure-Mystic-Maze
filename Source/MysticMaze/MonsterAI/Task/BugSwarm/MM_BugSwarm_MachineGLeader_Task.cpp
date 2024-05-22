// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MM_BugSwarm_MachineGLeader_Task.h"

UMM_BugSwarm_MachineGLeader_Task::UMM_BugSwarm_MachineGLeader_Task()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMM_BugSwarm_MachineGLeader_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

    return EBTNodeResult::Type();
}

void UMM_BugSwarm_MachineGLeader_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}


/*
1 1 1 0 1 1 1
1 1 1 0 1 1 1
1 1 1 0 1 1 1
0 0 0 0 0 0 0
1 1 1 0 1 1 1
1 1 1 0 1 1 1
1 1 1 0 1 1 1
*/
// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Grux/MM_Grux_DashATK_Task.h"

UMM_Grux_DashATK_Task::UMM_Grux_DashATK_Task()
{
}

EBTNodeResult::Type UMM_Grux_DashATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Type();
}

void UMM_Grux_DashATK_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Grux/MM_Grux_AngryModeChange_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Interface/MMGruxModeChangeInterface.h"

UMM_Grux_AngryModeChange_Task::UMM_Grux_AngryModeChange_Task()
{
}

EBTNodeResult::Type UMM_Grux_AngryModeChange_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	if (!OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool("AngryMode"))
	{
		IMMGruxModeChangeInterface* Grux = Cast<IMMGruxModeChangeInterface>(OwnerComp.GetAIOwner()->GetPawn());
		if (Grux)
		{
			Grux->AngryModeChange(true);

			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}

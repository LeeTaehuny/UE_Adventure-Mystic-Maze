// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Goblin/MM_Goblin_Howl_Task.h"

#include "Interface/MMGoblinHowlAnimationInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UMM_Goblin_Howl_Task::UMM_Goblin_Howl_Task()
{
	NodeName = "Howl Task";
}

EBTNodeResult::Type UMM_Goblin_Howl_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* GoblinBlackBoard = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (GoblinBlackBoard)
	{
		IMMGoblinHowlAnimationInterface* Goblin = Cast<IMMGoblinHowlAnimationInterface>(OwnerComp.GetAIOwner()->GetPawn());
		if (Goblin)
		{
			Goblin->PlayingHowl();
			GoblinBlackBoard->SetValueAsBool("PlayerFinded", true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Goblin Howl Task Is Not Blackboard"));
	}

	return EBTNodeResult::Failed;
}

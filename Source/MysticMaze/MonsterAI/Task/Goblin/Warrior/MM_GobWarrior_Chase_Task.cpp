// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Goblin/Warrior/MM_GobWarrior_Chase_Task.h"

#include "Interface/MMWarriorATKMontageInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMM_GobWarrior_Chase_Task::UMM_GobWarrior_Chase_Task()
{
	NodeName = "Warrior Chase";
}

EBTNodeResult::Type UMM_GobWarrior_Chase_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    if (Result == EBTNodeResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

	PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
	if (!PlayerData)
	{
		return EBTNodeResult::Failed;
	}

    bNotifyTick = true;

	return EBTNodeResult::InProgress;
}

void UMM_GobWarrior_Chase_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(PlayerData);
	MoveRequest.SetAcceptanceRadius(100.0f);
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);
	MoveRequest.SetReachTestIncludesAgentRadius(false);
	MoveRequest.SetReachTestIncludesGoalRadius(true);
	MoveRequest.SetCanStrafe(false);

	IMMWarriorATKMontageInterface* ATKNord = Cast<IMMWarriorATKMontageInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (ATKNord && ATKNord->PlayingAnimation())
	{
		ACharacter* move = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if (move)
		{
			move->GetCharacterMovement()->StopMovementImmediately();
		}
	}

	FPathFollowingRequestResult MoveResult = OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
	if (MoveResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if (ATKNord)
		{
			ATKNord->WarriorNormalATK();
		}

		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
		OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);
	}
}

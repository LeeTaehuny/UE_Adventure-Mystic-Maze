// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Goblin/Wizard/MM_Wizard_Repositioning_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Kismet/KismetMathLibrary.h"

UMM_Wizard_Repositioning_Task::UMM_Wizard_Repositioning_Task()
{
    NodeName = "Repositioning";
}

EBTNodeResult::Type UMM_Wizard_Repositioning_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    FVector Direction = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() - PlayerData->GetActorLocation();
    Direction = Direction.GetSafeNormal();

    float TargetDistance = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
    if (TargetDistance >= 500)
    {
        return EBTNodeResult::Failed;
    }
    else
    {
        Repositioning_Location = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()
            + (Direction * 1000);

        bNotifyTick = true;
        return EBTNodeResult::InProgress;
    }
}

void UMM_Wizard_Repositioning_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalLocation(Repositioning_Location);
    MoveRequest.SetAcceptanceRadius(150.0f);
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAllowPartialPath(true);
    MoveRequest.SetReachTestIncludesAgentRadius(false);
    MoveRequest.SetReachTestIncludesGoalRadius(true);
    MoveRequest.SetCanStrafe(false);

    FPathFollowingRequestResult MoveResult = OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
    if (MoveResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
        OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);
    }
}

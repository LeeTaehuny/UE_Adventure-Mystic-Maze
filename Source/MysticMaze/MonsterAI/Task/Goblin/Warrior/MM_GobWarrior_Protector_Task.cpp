// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Goblin/Warrior/MM_GobWarrior_Protector_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/MMWarriorATKMontageInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMM_GobWarrior_Protector_Task::UMM_GobWarrior_Protector_Task()
{
}

EBTNodeResult::Type UMM_GobWarrior_Protector_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

void UMM_GobWarrior_Protector_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalLocation(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector("ProtectLocation"));
    MoveRequest.SetAcceptanceRadius(50.0f);
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetAllowPartialPath(true);
    MoveRequest.SetReachTestIncludesAgentRadius(false);
    MoveRequest.SetReachTestIncludesGoalRadius(true);
    MoveRequest.SetCanStrafe(false);

    float TargetDistance = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
    if (TargetDistance <= 150.0f)
    {
        IMMWarriorATKMontageInterface* ATKNord = Cast<IMMWarriorATKMontageInterface>(OwnerComp.GetAIOwner()->GetPawn());
        if (ATKNord)
        {
            ACharacter* move = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
            if (move)
            {
                move->GetCharacterMovement()->StopMovementImmediately();
            }

            FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
            OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);

            ATKNord->WarriorNormalATK();
        }
    }

    FPathFollowingRequestResult MoveResult = OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
    if (MoveResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
        OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);
    }

    //APawn* PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
    
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/MM_Monster_Patrol_Task.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UMM_Monster_Patrol_Task::UMM_Monster_Patrol_Task()
{
}

EBTNodeResult::Type UMM_Monster_Patrol_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    if (Result == EBTNodeResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    // AI컨트롤러가 컨트롤하고 있는 폰을 가져옵니다.
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    // 네비게이션 시스템을 가져옵니다.
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (!NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    // ControllingPawn은 NPC이므로, 인터페이스로 캐스팅되는지 확인 후 인터페이스를 통해 필요한 값을 가져도오록 합니다.
    //IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
    //if (!AIPawn)
    //{ 
    //    return EBTNodeResult::Failed;
    //}
    
    // 탐색 거리를 설정합니다.
    float PatrolRadius = 2000;

    // 목적지 위치를 설정합니다.
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector("SpawnLocation");
    FNavLocation NextPatrolPos;

    // GetRandomPointInNavigableRadius : 시작지점(Origin)으로부터 일정거리(PatrolRadius)의 범위 내에 있는 랜덤한 위치(NextPatrolPos)를 뽑아준다. 
    if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", NextPatrolPos);

        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}

void UMM_Monster_Patrol_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

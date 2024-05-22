// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MMFindLeaderTaskNode.h"

#include "Monster/MMBugSwarm.h"
#include "Collision/MMCollision.h"
#include "BehaviorTree/BlackboardComponent.h"

UMMFindLeaderTaskNode::UMMFindLeaderTaskNode()
{
}

EBTNodeResult::Type UMMFindLeaderTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	FVector SpawnLocation = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector("SpawnLocation");

	TArray<FOverlapResult> Overlaps;
	FQuat Rotation = FQuat::Identity; // 회전 없음
	ECollisionChannel TraceChannel = ECC_WorldStatic; // 충돌 채널
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(8000.0f); // 구 형태의 충돌체 (반지름: 200)
	FCollisionQueryParams Params;

	AMMBugSwarmAIController* Mydata = Cast<AMMBugSwarmAIController>(OwnerComp.GetAIOwner());
	if (Mydata)
	{
		Mydata->GetBlackboardComponent()->SetValueAsObject("MyLeader", OwnerComp.GetAIOwner()->GetPawn());
		Mydata->GetBlackboardComponent()->SetValueAsBool("LeaderItMe", true);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	if (GetWorld()->OverlapMultiByChannel(
		Overlaps,		// 충돌한 액터들의 정보
		SpawnLocation,	// 탐색할 위치
		Rotation,		// 회전 정보 (회전 없음)
		CHANNEL_MMACTION,	// 충돌 채널
		CollisionShape, // 충돌체
		Params))
	{
		for (const FOverlapResult& Data : Overlaps)
		{
			if (!Data.GetActor()->ActorHasTag(TEXT("BugSwarm")))
			{
				continue;
			}

			APawn* changeBug = Cast<APawn>(Data.GetActor());
			if (changeBug)
			{
				AMMBugSwarmAIController* GetAiController = Cast<AMMBugSwarmAIController>(changeBug->GetController());
				if (GetAiController)
				{
					GetAiController->GetBlackboardComponent()->SetValueAsObject("MyLeader", OwnerComp.GetAIOwner()->GetPawn());
					Mydata->AddSoldier(GetAiController);
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}

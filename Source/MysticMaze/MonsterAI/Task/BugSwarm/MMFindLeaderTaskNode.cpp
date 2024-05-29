// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MMFindLeaderTaskNode.h"

#include "Monster/MMBugSwarm.h"
#include "Collision/MMCollision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MMMonsterATKModeInterface.h"

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
	//FCollisionShape CollisionShape = FCollisionShape::MakeSphere(8000.0f); 
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(4000, 4000, 1000)); 
	FCollisionQueryParams Params;

	AMMBugSwarmAIController* Mydata = Cast<AMMBugSwarmAIController>(OwnerComp.GetAIOwner());
	IMMMonsterATKModeInterface* MyPawnData = Cast<IMMMonsterATKModeInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (Mydata)
	{
		Mydata->GetBlackboardComponent()->SetValueAsObject("MyLeader", OwnerComp.GetAIOwner()->GetPawn());
		Mydata->GetBlackboardComponent()->SetValueAsBool("LeaderItMe", true);

		MyPawnData->SetATKMode(true);
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
		//DrawDebugBox(
		//	GetWorld(),
		//	SpawnLocation,
		//	CollisionShape.GetBox(),
		//	FColor::Red, // 색상
		//	false, // 영구 여부 (true로 설정하면 디버그 도형이 영구히 남음)
		//	5.0f // 지속 시간
		//);

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
				if (GetAiController &&
					!GetAiController->GetBlackboardComponent()->GetValueAsObject("MyLeader"))
				{
					GetAiController->GetBlackboardComponent()->SetValueAsObject("MyLeader", OwnerComp.GetAIOwner()->GetPawn());
					Mydata->AddSoldier(GetAiController);
				}
			}
		}

		for (AMMBugSwarmAIController* monC : Mydata->GetSoldiersData())
		{
			//monC->GetBlackboardComponent()->SetValueAsBool("ATK_Mode", true);

			IMMMonsterATKModeInterface* SoldierDatas = Cast<IMMMonsterATKModeInterface>(monC->GetPawn());
			if (SoldierDatas)
			{
				SoldierDatas->SetATKMode(true);
			}
		}
	}

	return EBTNodeResult::Succeeded;
}

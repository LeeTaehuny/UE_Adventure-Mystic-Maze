// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MM_BugSwarmLeader_RelayATK_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/MMMonsterBase.h"

UMM_BugSwarmLeader_RelayATK_Task::UMM_BugSwarmLeader_RelayATK_Task()
{
	TimerTicktok = 0;
}

EBTNodeResult::Type UMM_BugSwarmLeader_RelayATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	MyData = OwnerComp.GetAIOwner()->GetPawn();
	//bNotifyTick = true;

	APawn* PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));

	AMMBugSwarmAIController* MyLeaderController = Cast<AMMBugSwarmAIController>(MyData->GetController());

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(MyData->GetActorLocation(), PlayerData->GetActorLocation());
	MyData->SetActorRotation(NewRotation);

	FHitResult HitResult;
	FVector Start = MyData->GetActorLocation();
	FVector Direction = MyData->GetActorRightVector();
	FVector End = Start + (Direction * 4000);
	ECollisionChannel TraceChannel = ECC_Visibility;

	FCollisionQueryParams TraceParams(TEXT("MyTrace"), true, PlayerData);
	if (MyLeaderController)
	{
		for (int i = 0; i < MyLeaderController->GetSoldiersData().Num(); i++)
		{
			TraceParams.AddIgnoredActor(MyLeaderController->GetSoldiersData()[i]->GetPawn());
		}
	}

	int Interval = 150; // 몬스터 배치 간격 수치
	rightLine = 0;
	// 오른쪽의 공간 확인
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, TraceParams))
	{
		float Distance = HitResult.ImpactPoint.X - Start.X;
		Distance = FMath::Abs(Distance);

		rightLine = Distance / Interval;
		if (rightLine >= 5)
		{
			rightLine = 5;
		}
	}
	else
	{
		rightLine = 5;
	}

	// 이곳에서 배치시킬 몬스터들에게 정보를 전달합니다.
	FVector RLocation = MyData->GetActorLocation() + (MyData->GetActorRightVector() * Interval);
	MopsData = MyLeaderController->GetSoldiersData();
	int IntervalCount = 0;
	int OOO = 0;

	for (int i = 0; i < MopsData.Num(); i++)
	{
		// 리더의 대우를 고민해봐야 함
		if (MopsData[i] && MopsData[i]->GetBlackboardComponent()->GetValueAsBool("LeaderItMe"))
		{
			MopsData[i]->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", true);
			continue;
		}

		IntervalCount++;
		if (IntervalCount >= rightLine)
		{
			IntervalCount = 0;
			OOO++;
			RLocation = MyData->GetActorLocation() + (-MyData->GetActorForwardVector() * Interval) * OOO;
		}

		MopsData[i]->GetBlackboardComponent()->SetValueAsVector("RehabilitationLocation", RLocation);
		RLocation += (MyData->GetActorRightVector() * Interval);
		MopsData[i]->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", true);
	}

	UWorld* world = GetWorld();
	TimerTicktok += world->DeltaTimeSeconds;
	if (TimerTicktok >= 1.5f)
	{
		TimerTicktok = 0;
		for (int i = 0; i < MopsData.Num(); i++)
		{
			MopsData[i]->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", false);
			MopsData[i]->GetBlackboardComponent()->SetValueAsBool("bSpeedAttack", true);
		}

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Type();
}

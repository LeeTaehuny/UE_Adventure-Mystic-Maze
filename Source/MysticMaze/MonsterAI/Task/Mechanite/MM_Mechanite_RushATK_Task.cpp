// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Mechanite/MM_Mechanite_RushATK_Task.h"

#include "AIController.h"
#include "Interface/MMMechaniteRushATKInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MMMechaniteAnimInterface.h"

UMM_Mechanite_RushATK_Task::UMM_Mechanite_RushATK_Task()
{
}

EBTNodeResult::Type UMM_Mechanite_RushATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Monster = OwnerComp.GetAIOwner()->GetPawn();
	IMMMechaniteRushATKInterface* MonsterInterface = Cast<IMMMechaniteRushATKInterface>(Monster);
	if (MonsterInterface)
	{
		MonsterInterface->AnimEnd(true);
		MonsterInterface->SettingMoveSpeed(4);
		MonsterInterface->StartMechaniteAnim(2);
		MonsterInterface->ChangeCollision();
	}

	FHitResult HitResult;
	APawn* Player = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));	
	{
		// 레이 트레이스를 준비하기 위한 밑작업
		// 플레이어의 위치에 시작점을 설정
		FVector Start = Player->GetActorLocation();

		// 플레이어의 위치와 버그스웜의 위치를 통해 방향 정보를 얻음
		FVector Direction = (Player->GetActorLocation() - Monster->GetActorLocation()).GetSafeNormal();

		// 얻은 방향에 플레이어의 위치를 곱하여 플레이어와 버그스웜 사이의 직선 거리를 구한 후
		// 얻은 직선 거리의 뒤쪽 거리 5M 까지를 End 지점으로 설정
		FVector End = Start + (Direction * 600);

		// TODO : 나중에 수정이 필요할 수 있음
		End.Z = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().Z;

		// 검색할 채널과 충돌결과를 제거할 액터들을 설정
		// 이곳에서는 버그스웜과 플레이어를 제거
		// TODO : 추후 몬스터가 추가될 것을 생각하여 해결해야 함
		ECollisionChannel TraceChannel = ECC_WorldStatic;
		FCollisionQueryParams TraceParams(TEXT("MyTrace"), true, Player);

		// 레이 트레이스를 쏴서 충돌이 발생했을 경우
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, TraceParams))
		{
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector("Rush_ATK_Location", HitResult.ImpactPoint);
			// 충돌이 발생한 지점을 임팩트 포인트로 저장
			//HitLocation = HitResult.ImpactPoint;
		}
		else
		{
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector("Rush_ATK_Location", End);
			// 충돌이 발생하지 않았을 경우 End 지점을 저장
			//HitLocation = End;
		}
	}
	bNotifyTick = true;

	//return EBTNodeResult::Type();
	return EBTNodeResult::InProgress;
}

void UMM_Mechanite_RushATK_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector("Rush_ATK_Location"));
	MoveRequest.SetAcceptanceRadius(50.0f);
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);
	MoveRequest.SetReachTestIncludesAgentRadius(false);
	MoveRequest.SetReachTestIncludesGoalRadius(true);
	MoveRequest.SetCanStrafe(false);

	FPathFollowingRequestResult MoveResult = OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
	IMMMechaniteRushATKInterface* MonsterInterface = Cast<IMMMechaniteRushATKInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (MoveResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if (MonsterInterface)
		{
			MonsterInterface->AnimEnd(false);
			MonsterInterface->SettingMoveSpeed(1);
			MonsterInterface->EndMechaniteAnim();
		}

		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bNormalATK", true);
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bRushATK", false);

		OwnerComp.GetAIOwner()->GetPawn()->GetMovementComponent();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

EBTNodeResult::Type UMM_Mechanite_RushATK_Task::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Aborted;
}

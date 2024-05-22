// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/BugSwarm/MM_BugSwarm_SpeedATK_Task.h"

#include "MonsterAI/AIController/MMBugSwarmAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UMM_BugSwarm_SpeedATK_Task::UMM_BugSwarm_SpeedATK_Task()
{
	
}

EBTNodeResult::Type UMM_BugSwarm_SpeedATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	bNotifyTick = true;
	FAIMoveRequest MoveRequest;
	FHitResult HitResult;
	MyController = OwnerComp.GetAIOwner();

	BugSwarmCapusulData = OwnerComp.GetAIOwner()->GetCharacter()->GetCapsuleComponent();
	
	{
		// 플레이어이의 데이터
		APawn* MyData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
		if (!MyData)
		{
			return EBTNodeResult::Failed;
		}

		{
			// 리더의 정보를 받아옴
			APawn* MyLeader = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("MyLeader"));

			// 받아온 리더의 정보로 컨트롤러의 정보를 받아옴
			AMMBugSwarmAIController* MyLeaderController = Cast<AMMBugSwarmAIController>(MyLeader->GetController());

			// 레이 트레이스를 준비하기 위한 밑작업
			// 플레이어의 위치에 시작점을 설정
			FVector Start = MyData->GetActorLocation();

			// 플레이어의 위치와 버그스웜의 위치를 통해 방향 정보를 얻음
			FVector Direction = (MyData->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()).GetSafeNormal();

			// 얻은 방향에 플레이어의 위치를 곱하여 플레이어와 버그스웜 사이의 직선 거리를 구한 후
			// 얻은 직선 거리의 뒤쪽 거리 5M 까지를 End 지점으로 설정
			FVector End = MyData->GetActorLocation() + (Direction * 500);

			// 버그스웜과 플레이어의 높이 차이를 제거하기 위한 함수
			// TODO : 나중에 수정이 필요할 수 있음
			End.Z = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().Z;

			// 검색할 채널과 충돌결과를 제거할 액터들을 설정
			// 이곳에서는 버그스웜과 플레이어를 제거
			// TODO : 추후 몬스터가 추가될 것을 생각하여 해결해야 함
			ECollisionChannel TraceChannel = ECC_WorldStatic;
			FCollisionQueryParams TraceParams(TEXT("MyTrace"), true, MyData);
			if (MyLeaderController)
			{
				for (int i = 0; i < MyLeaderController->GetSoldiersData().Num(); i++)
				{
					TraceParams.AddIgnoredActor(MyLeaderController->GetSoldiersData()[i]->GetPawn());
				}
			}

			// 레이 트레이스를 쏴서 충돌이 발생했을 경우
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, TraceParams))
			{
				// 충돌이 발생한 지점을 임팩트 포인트로 저장
				HitLocation = HitResult.ImpactPoint;
			}
			else
			{
				// 충돌이 발생하지 않았을 경우 End 지점을 저장
				HitLocation = End;
			}		

			
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Type();
}

void UMM_BugSwarm_SpeedATK_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// move to 함수에 값을 전달하기 위한 변수 선언
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(HitLocation);
	MoveRequest.SetAcceptanceRadius(25.0f);
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);
	MoveRequest.SetReachTestIncludesAgentRadius(true);
	MoveRequest.SetReachTestIncludesGoalRadius(true);
	MoveRequest.SetCanStrafe(true);
	
	BugSwarmCapusulData->SetCollisionProfileName(TEXT("MMTrigger"));
	FPathFollowingRequestResult MoveResult = MyController->MoveTo(MoveRequest);

	// move to 함수의 반환 값에 따라 종료를 결정
	// 완료 되었을 경우 공격을 종료
	if (MoveResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bSpeedAttack", false);
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", false);
		
		// 공격이 종료된 것이 리더일 경우 바로 재탐색이 가능하도록 설정
		if (OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool("LeaderItMe"))
		{
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bRehabilitation", true);
		}
		// 공격이 종료된 것이 리더가 아닐 경우 제자리에서 대기하도록 설정
		else
		{
		}

		// 테스크 종료 전 콜리전 프리셋 변경
		BugSwarmCapusulData->SetCollisionProfileName(TEXT("MMCapsule"));

		// 노드를 바로 끝내기 위한 실패 반환
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

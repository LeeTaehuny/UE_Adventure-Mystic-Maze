// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Mechanite/MM_Mechanite_NormalATK_Task.h"

#include "Interface/MMMechaniteAnimInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UMM_Mechanite_NormalATK_Task::UMM_Mechanite_NormalATK_Task()
{

}

EBTNodeResult::Type UMM_Mechanite_NormalATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	// 공격 시작시 플레이어를 볼 수 있도록 설정
	PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
	if (!PlayerData)
	{
		return EBTNodeResult::Failed;
	}

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UMM_Mechanite_NormalATK_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(PlayerData);
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

		// 몽타주 애니메이션 재생
		IMMMechaniteAnimInterface* MonsterInterface = Cast<IMMMechaniteAnimInterface>(OwnerComp.GetAIOwner()->GetPawn());
		if (MonsterInterface)
		{
			MonsterInterface->AnimEnd(true);
			MonsterInterface->StartMechaniteAnim(1);
		}
	}
	else if (MoveResult.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		{
			// 몽타주 애니메이션 재생
			IMMMechaniteAnimInterface* MonsterInterface = Cast<IMMMechaniteAnimInterface>(OwnerComp.GetAIOwner()->GetPawn());
			if (MonsterInterface)
			{
				MonsterInterface->AnimEnd(false);
				MonsterInterface->EndMechaniteAnim();
			}
		}
	}

	// 돌진 스킬의 쿨타임 : 쿨타임이 되면 바로바로 움직이게 함
	IMMMechaniteAnimInterface* MonsterInterface = Cast<IMMMechaniteAnimInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (MonsterInterface && MonsterInterface->GetCoolOn())
	{
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bNormalATK", false);
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("bRushATK", true);
		MonsterInterface->SetCoolOn(false);

		if (MonsterInterface)
		{
			MonsterInterface->MovingStop();
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

}

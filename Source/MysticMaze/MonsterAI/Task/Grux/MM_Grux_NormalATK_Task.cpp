// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Grux/MM_Grux_NormalATK_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/MMGruxNormalATKInterface.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UMM_Grux_NormalATK_Task::UMM_Grux_NormalATK_Task()
{
}

EBTNodeResult::Type UMM_Grux_NormalATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("LeftHook_RightHook", true);
	// 공격 시작시 플레이어를 볼 수 있도록 설정
	PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
	if (!PlayerData)
	{
		return EBTNodeResult::Failed;
	}

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UMM_Grux_NormalATK_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	IMMGruxNormalATKInterface* MonsterInterface = Cast<IMMGruxNormalATKInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (MonsterInterface->MontagePlaying())
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
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
		OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);

		//OwnerComp.GetAIOwner()->GetPawn();
		//	GetCharacterMovement()->StopMovementImmediately();

		// 몽타주 애니메이션 재생
		//IMMGruxNormalATKInterface* MonsterInterface = Cast<IMMGruxNormalATKInterface>(OwnerComp.GetAIOwner()->GetPawn());
		if (OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool("LeftHook_RightHook"))
		{
			MonsterInterface->RightHook();
		}
		else
		{
			MonsterInterface->LeftHook();
		}
	}

}

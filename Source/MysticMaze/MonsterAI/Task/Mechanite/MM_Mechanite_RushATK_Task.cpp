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
		// ���� Ʈ���̽��� �غ��ϱ� ���� ���۾�
		// �÷��̾��� ��ġ�� �������� ����
		FVector Start = Player->GetActorLocation();

		// �÷��̾��� ��ġ�� ���׽����� ��ġ�� ���� ���� ������ ����
		FVector Direction = (Player->GetActorLocation() - Monster->GetActorLocation()).GetSafeNormal();

		// ���� ���⿡ �÷��̾��� ��ġ�� ���Ͽ� �÷��̾�� ���׽��� ������ ���� �Ÿ��� ���� ��
		// ���� ���� �Ÿ��� ���� �Ÿ� 5M ������ End �������� ����
		FVector End = Start + (Direction * 600);

		// TODO : ���߿� ������ �ʿ��� �� ����
		End.Z = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().Z;

		// �˻��� ä�ΰ� �浹����� ������ ���͵��� ����
		// �̰������� ���׽����� �÷��̾ ����
		// TODO : ���� ���Ͱ� �߰��� ���� �����Ͽ� �ذ��ؾ� ��
		ECollisionChannel TraceChannel = ECC_WorldStatic;
		FCollisionQueryParams TraceParams(TEXT("MyTrace"), true, Player);

		// ���� Ʈ���̽��� ���� �浹�� �߻����� ���
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, TraceParams))
		{
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector("Rush_ATK_Location", HitResult.ImpactPoint);
			// �浹�� �߻��� ������ ����Ʈ ����Ʈ�� ����
			//HitLocation = HitResult.ImpactPoint;
		}
		else
		{
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector("Rush_ATK_Location", End);
			// �浹�� �߻����� �ʾ��� ��� End ������ ����
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

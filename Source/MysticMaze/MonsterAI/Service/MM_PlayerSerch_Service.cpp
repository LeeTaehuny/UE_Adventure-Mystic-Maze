// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Service/MM_PlayerSerch_Service.h"

#include "Collision/MMCollision.h"
#include "Interface/MMMonsterATKModeInterface.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UMM_PlayerSerch_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
	float PlayerAndMonster = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());

	IMMMonsterATKModeInterface* PLDAta = Cast<IMMMonsterATKModeInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (PlayerAndMonster <= 700)
	{
		PLDAta->SetATKMode(true);
	}

}

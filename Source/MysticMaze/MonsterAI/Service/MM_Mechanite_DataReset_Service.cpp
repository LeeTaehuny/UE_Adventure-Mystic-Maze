// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Service/MM_Mechanite_DataReset_Service.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MMMechainteCoolDownInterface.h"

void UMM_Mechanite_DataReset_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (!OwnerBlackboard->GetValueAsBool("ATK_Mode"))
	{
		OwnerBlackboard->SetValueAsBool("bNormalATK", true);
		OwnerBlackboard->SetValueAsBool("bRushATK", false);
		OwnerBlackboard->SetValueAsVector("Rush_ATK_Location", FVector());

		IMMMechainteCoolDownInterface* MechainteCoolDown = Cast<IMMMechainteCoolDownInterface>(OwnerComp.GetAIOwner()->GetPawn());
		if (MechainteCoolDown)
		{
			MechainteCoolDown->ResetCoolDownbool(false);
			MechainteCoolDown->ResetCoolDownFloat(0.0f);
		}
	}
}

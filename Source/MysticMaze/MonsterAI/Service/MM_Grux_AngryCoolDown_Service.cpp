// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Service/MM_Grux_AngryCoolDown_Service.h"

#include "Interface/MMGruxCoolDownInterface.h"
#include "AIController.h"

void UMM_Grux_AngryCoolDown_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	IMMGruxCoolDownInterface* Grux = Cast<IMMGruxCoolDownInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (Grux && Grux->GetCoolOn())
	{
		float GeCoolTime = Grux->GetCoolDown();

		if (GeCoolTime <= 10)
		{
			GeCoolTime += DeltaSeconds;
			Grux->SetCoolDown(GeCoolTime);
			UE_LOG(LogTemp, Display, TEXT("Time : %f"), GeCoolTime);
		}
		else
		{
			Grux->SetCoolOn(true);
		}
	}
}

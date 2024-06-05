// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Service/MM_WizardDealer_Service.h"

#include "Interface/MMWizardDealerServiceInterface.h"
//#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UMM_WizardDealer_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	IMMWizardDealerServiceInterface* Wizard = Cast<IMMWizardDealerServiceInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (Wizard)
	{
		if (!Wizard->GetNormalATK())
		{
			float MinSecond = Wizard->GetMInSecondATK();
			MinSecond += DeltaSeconds;
			Wizard->SetMInSecondATK(MinSecond);
			if (Wizard->GetMInSecondATK() >= Wizard->GetMaxSecondATK())
			{
				Wizard->SetNormalATK(true);
				Wizard->SetMInSecondATK(0.0f);
			}
		}
	}
}

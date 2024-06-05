// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Task/Goblin/Wizard/MM_GobWizard_NormalATK_Task.h"

#include "Interface/MMWizardATKMontageInterface.h"

#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UMM_GobWizard_NormalATK_Task::UMM_GobWizard_NormalATK_Task()
{
	NodeName = "Wizard ATK";
    bNotifyTick = true;
}

EBTNodeResult::Type UMM_GobWizard_NormalATK_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    if (Result == EBTNodeResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
    if (!PlayerData)
    {
        return EBTNodeResult::Failed;
    }

    // 일반 공격
    IMMWizardATKMontageInterface* GoblinWizard = Cast<IMMWizardATKMontageInterface>(OwnerComp.GetAIOwner()->GetPawn());
    if (GoblinWizard)
    {
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
        OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);

        GoblinWizard->StartATKMonatage(1);
    }

    bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UMM_GobWizard_NormalATK_Task::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    IMMWizardATKMontageInterface* GoblinWizard = Cast<IMMWizardATKMontageInterface>(OwnerComp.GetAIOwner()->GetPawn());
    if (GoblinWizard)
    {
        if (GoblinWizard->GetMontagePlaying())
        {
            FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
            OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);
        }
        else
        {
            GoblinWizard->SetNormalATK(false);
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        }
    }
}

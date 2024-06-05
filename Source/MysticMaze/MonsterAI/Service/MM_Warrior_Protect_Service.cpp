// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/Service/MM_Warrior_Protect_Service.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/MMWarriorATKMontageInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMM_Warrior_Protect_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerData = Cast<APawn>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));

	float TargetDistance = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
	if (TargetDistance <= 150.0f)
	{
		IMMWarriorATKMontageInterface* ATKNord = Cast<IMMWarriorATKMontageInterface>(OwnerComp.GetAIOwner()->GetPawn());
		if (ATKNord)
		{
			ACharacter* move = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
			if (move)
			{
				move->GetCharacterMovement()->StopMovementImmediately();
			}

			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), PlayerData->GetActorLocation());
			OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(NewRotation);

			ATKNord->WarriorNormalATK();
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMGoblinWizardAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AMMGoblinWizardAIController::AMMGoblinWizardAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMMGoblinWizardAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BegAIBehavior)
	{
		RunBehaviorTree(BegAIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Blackboard->SetValueAsObject("TargetPlayer", PlayerPawn);
		}
	}
}

void AMMGoblinWizardAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI/AIController/MMMechaniteAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interface/MMMechaniteAIInterface.h"

AMMMechaniteAIController::AMMMechaniteAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	MyBlackboardData = CreateDefaultSubobject<UBlackboardComponent>(TEXT("/Script/AIModule.BlackboardData'/Game/MysticMaze/Monster/Mechanite/AI/BB_Mechanite.BB_Mechanite'"));
}

void AMMMechaniteAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BegAIBehavior)
	{
		RunBehaviorTree(BegAIBehavior);

		if (BegAIBehavior->BlackboardAsset)
		{
			MyBlackboardData->InitializeBlackboard(*(BegAIBehavior->BlackboardAsset));

		}

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Blackboard->SetValueAsObject("TargetPlayer", PlayerPawn);
			Blackboard->SetValueAsBool("bNormalATK", true);


		}
	}
}

void AMMMechaniteAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IMMMechaniteAIInterface* OwnerData = Cast<IMMMechaniteAIInterface>(GetPawn());
	if (OwnerData)
	{
		Blackboard->SetValueAsBool("ATK_Mode", OwnerData->GetATKMode());
	}
}

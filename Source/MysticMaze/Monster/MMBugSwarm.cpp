// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMBugSwarm.h"

//#include "Monster/MMBugSwarmAIController.h"

AMMBugSwarm::AMMBugSwarm()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BugSwarm/Meshes/Buff_Blue.Buff_Blue'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		RootComponent = GetMesh();
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/BugSwarm/Animation/ABP_BugSwarm.ABP_BugSwarm_C'"));
		if (AnimInstanceClassRef.Class)
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
		}
	}

	//this->AIControllerClass = AMMBugSwarmAIController::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 애니메이션 블루프린트용 uint8 변수 선언?
	{
		Die = false;
		Spawn = true;
	}
}

void AMMBugSwarm::BeginPlay()
{
}

void AMMBugSwarm::Tick(float DeltaSeconds)
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMechanite.h"
#include "MonsterAI/AIController/MMMechaniteAIController.h"

AMMMechanite::AMMMechanite()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Mechanite/Meshes/Minion_Lane_Super_Dawn.Minion_Lane_Super_Dawn'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -120.0f), FRotator(0.0f, -90.0f, 0.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Mechanite/Animations/ABP_Mechanite.ABP_Mechanite_C'"));
		if (AnimInstanceClassRef.Class)
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
		}

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FClassFinder<AMMMechaniteAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Mechanite/AI/BP_MechaniteAIController.BP_MechaniteAIController_C'"));
	if (BehaviorRef.Succeeded())
	{
		this->AIControllerClass = BehaviorRef.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}

void AMMMechanite::BeginPlay()
{
	Super::BeginPlay();
}

void AMMMechanite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

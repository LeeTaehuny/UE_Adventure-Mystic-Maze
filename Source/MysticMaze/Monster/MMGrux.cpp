// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGrux.h"

#include "MonsterAI/AIController/MMGruxAIController.h"

AMMGrux::AMMGrux()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -120.0f), FRotator(0.0f, -90.0f, 0.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Grux/Animation/ABP_Grux.ABP_Grux_C'"));
		if (AnimInstanceClassRef.Class)
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
		}

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FClassFinder<AMMGruxAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Grux/AI/BP_GruxAIController.BP_GruxAIController_C'"));
	if (BehaviorRef.Succeeded())
	{
		this->AIControllerClass = BehaviorRef.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
	
	// 캐릭터의 걷는 속도 설정
	//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ATK_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Rush Check"));
	ATK_Collision->SetupAttachment(GetMesh());
	ATK_Collision->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMMGrux::BeginPlay()
{
	Super::BeginPlay();
}

void AMMGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

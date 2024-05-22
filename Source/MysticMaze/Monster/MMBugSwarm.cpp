// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMBugSwarm.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

AMMBugSwarm::AMMBugSwarm()
{
	PrimaryActorTick.bCanEverTick = true;
	bBugFlying = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BugSwarm/Meshes/Buff_Blue.Buff_Blue'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/BugSwarm/Animation/ABP_BugSwarm.ABP_BugSwarm_C'"));
		if (AnimInstanceClassRef.Class)
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
		}

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("MMCapsule"));
	}

	static ConstructorHelpers::FClassFinder<AMMBugSwarmAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/BugSwarm/AI/BP_BugSwarmAIController.BP_BugSwarmAIController_C'"));
	if (BehaviorRef.Succeeded())
	{
		this->AIControllerClass = BehaviorRef.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
	
	// 애니메이션 블루프린트용 uint8 변수 선언
	bDie = false;
	bSpawn = true;
	bRandomDieMotion = FMath::RandRange(0, 2);

}

void AMMBugSwarm::BeginPlay()
{
	Super::BeginPlay();

	TickTime = 0.0f;
	FVector Start = -GetActorUpVector();
	FVector End = Start * 1000.f;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	// 충돌한 물체가 있을 경우
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		ZLocation = HitResult.Location.Z;
	}
}

void AMMBugSwarm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO : 레이케스트를 통해 바닥의 위치를 파악하고 러프 기능을 사용하여
	//		  비행 기능을 구현하기로 함
	if (!bBugFlying)
	{
		TickTime += DeltaTime;
		if (TickTime >= 4.0f)
		{
			TickTime = 0.0f;
			FVector Start = -GetActorUpVector();
			FVector End = Start * 1000.f;
			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			// 충돌한 물체가 있을 경우
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
			{
				ZLocation = HitResult.Location.Z;
			}
		}
	}
	else
	{
		TickTime = 0;
		FVector flyingLocation = GetActorLocation();
		flyingLocation.Z = ZLocation;

		SetActorLocation(flyingLocation);
	}
}

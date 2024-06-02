// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGoblinWarrior.h"

#include "MonsterAI/AIController/MMGoblinWarriorAIController.h"

#include "Components/CapsuleComponent.h"
#include "Collision/MMCollision.h"

AMMGoblinWarrior::AMMGoblinWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Goblin/Animations/ABP_GoblinWarrior.ABP_GoblinWarrior_C'"));
		if (AnimInstanceClassRef.Class)
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
		}

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinWarriorAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/Warrior/AI/BP_GoblinWarriorAIController.BP_GoblinWarriorAIController_C'"));
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

void AMMGoblinWarrior::BeginPlay()
{
	Super::BeginPlay();

	ATK_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterBase::ATKBeginOverlap);
	ATK_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterBase::ATKEndOverlap);
}

void AMMGoblinWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMMGoblinWarrior::ATKChecking()
{
	// 충돌 결과를 반환하기 위한 배열
	TArray<FHitResult> OutHitResults;

	// 공격 반경
	float AttackRange = 100.0f;
	// 공격 체크를 위한 구체의 반지름
	float AttackRadius = 50.0f;

	// 충돌 탐지를 위한 시작 지점 (플레이어 현재 위치 + 전방 방향 플레이어의 CapsuleComponent의 반지름 거리)
	FVector Start = GetActorLocation() + (GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius());
	// 충돌 탐지 종료 지점 (시작지점 + 전방 방향의 공격 거리)
	FVector End = Start + (GetActorForwardVector() * AttackRange);
	// 파라미터 설정하기 (트레이스 태그 : Attack, 복잡한 충돌 처리 : false, 무시할 액터 : this) 
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	bool bHasHit = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bHasHit)
	{
		// TODO : 데미지 전달
		for (FHitResult Result : OutHitResults)
		{
			if (AMMMonsterBase* Monster = Cast<AMMMonsterBase>(Result.GetActor()))
			{
				continue;
			}

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.GetActor()->GetName());

			UGameplayStatics::ApplyDamage(Result.GetActor(),
				100.0f, GetController(),
				this,
				UDamageType::StaticClass());
		}
	}

	// Capsule 모양의 디버깅 체크
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
}
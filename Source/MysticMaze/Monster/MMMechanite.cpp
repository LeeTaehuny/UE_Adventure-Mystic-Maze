// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMechanite.h"
#include "MonsterAI/AIController/MMMechaniteAIController.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotify_MMBaseAttackCheck.h"
#include "Collision/MMCollision.h"
#include "Components/CapsuleComponent.h"

AMMMechanite::AMMMechanite()
{
	PrimaryActorTick.bCanEverTick = true;
	RushMontageEnd = false;
	RushATKCoolOn = false;

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

	// 캐릭터의 걷는 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ATK_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Rush Check"));
	ATK_Collision->SetupAttachment(GetMesh());
	ATK_Collision->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMMMechanite::BeginPlay()
{
	Super::BeginPlay();
	RushATKCoolDown_MaxTime = FMath::RandRange(5, 16);

	ATK_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterBase::ATKBeginOverlap);
	ATK_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterBase::ATKEndOverlap);
}

void AMMMechanite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!RushATKCoolOn)
	{
		RushATKCoolDown += DeltaTime;
		if (RushATKCoolDown >= RushATKCoolDown_MaxTime)
		{
			RushATKCoolDown = 0;
			RushATKCoolDown_MaxTime = FMath::RandRange(5, 16);

			RushATKCoolOn = true;
		}
	}
}

void AMMMechanite::RushATK_End(class UAnimMontage* Montage, bool IsEnded)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && RushMontageEnd)
	{
		FOnMontageEnded EndDelegate;
		switch (AKTChoice)
		{
		case 1:
			AnimInstance->Montage_Play(ATKNormal, 1);

			// 몽타주 재생 종료 바인딩
			EndDelegate.BindUObject(this, &AMMMechanite::RushATK_End);

			// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
			AnimInstance->Montage_SetEndDelegate(EndDelegate, ATKNormal);
			break;

		case 2:
			AnimInstance->Montage_Play(ATKRush, 1.5);

			// 몽타주 재생 종료 바인딩
			
			EndDelegate.BindUObject(this, &AMMMechanite::RushATK_End);

			// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
			AnimInstance->Montage_SetEndDelegate(EndDelegate, ATKRush);
			break;

		default:
			break;
		}
	}
}

void AMMMechanite::ATKChecking()
{
	// 충돌 결과를 반환하기 위한 배열
	TArray<FHitResult> OutHitResults;

	// 공격 반경
	float AttackRange = 100.0f;
	// 공격 체크를 위한 구체의 반지름
	float AttackRadius = 100.0f;

	// 충돌 탐지를 위한 시작 지점 (플레이어 현재 위치 + 전방 방향 플레이어의 CapsuleComponent의 반지름 거리)
	FVector Start = GetActorLocation() + (GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 2));
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

			UGameplayStatics::ApplyDamage(Result.GetActor(),
				100.0f, GetController(),
				this,
				UDamageType::StaticClass());

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.GetActor()->GetName());
		}
	}

	// Capsule 모양의 디버깅 체크
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
}

void AMMMechanite::StartMechaniteAnim(int INChoiceMontage)
{
	AKTChoice = INChoiceMontage;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			FOnMontageEnded EndDelegate;
			switch (AKTChoice)
			{
			case 1:

				AnimInstance->Montage_Play(ATKNormal, 1.0f);

				// 몽타주 재생 종료 바인딩
				EndDelegate.BindUObject(this, &AMMMechanite::RushATK_End);

				// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
				AnimInstance->Montage_SetEndDelegate(EndDelegate, ATKNormal);
				break;

			case 2:
				AnimInstance->Montage_Play(ATKRush, 1.7f);

				// 몽타주 재생 종료 바인딩
				EndDelegate.BindUObject(this, &AMMMechanite::RushATK_End);

				// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
				AnimInstance->Montage_SetEndDelegate(EndDelegate, ATKRush);
				break;

			default:
				break;
			}
		}
	}
}

void AMMMechanite::EndMechaniteAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(1.5f);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("MMCapsule"));
	}
}

void AMMMechanite::AnimEnd(bool INData)
{
	RushMontageEnd = INData;
}

void AMMMechanite::SettingMoveSpeed(float INDats)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * INDats;
}

void AMMMechanite::MovingStop()
{
	GetMovementComponent()->StopMovementImmediately();
}

void AMMMechanite::RushATKCheck()
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

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.GetActor()->GetName());
		}
	}

	// Capsule 모양의 디버깅 체크
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
}

void AMMMechanite::ChangeCollision()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MMTrigger"));
	ATK_Collision->SetCollisionProfileName(TEXT("MMLongATK"));
}


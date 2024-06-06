// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGrux.h"

#include "MonsterAI/AIController/MMGruxAIController.h"
#include "Animation/AnimMontage.h"
#include "Collision/MMCollision.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

AMMGrux::AMMGrux()
{
	PrimaryActorTick.bCanEverTick = true;

	bSpawn = true;
	bAngryModeChangeComplete = false;
	RushCoolOn = false;

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

	// 그룩스의 전투에 관련된 변수
	{
		bAngryMode = false;
		bNormalRightATK = false;
		bAngryMode = false; // 분노모드 테스트를 위한 임시 true : 기본 값 false
	}
}

void AMMGrux::BeginPlay()
{
	Super::BeginPlay();

	ATK_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterBase::ATKBeginOverlap);
	ATK_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterBase::ATKEndOverlap);
}

void AMMGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMMGrux::MonsterDieMontage()
{
	GetMesh()->GetAnimInstance()->Montage_Play(DieMontage);

	int RandomATKMotionStart = FMath::RandRange(1, 2);
	switch (RandomATKMotionStart)
	{
	case 1:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);
		break;

	case 2:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);
		break;

	default:
		break;
	}
}

void AMMGrux::Monsterdie()
{
	this->Destroy();
}

void AMMGrux::LeftHook()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopMovementImmediately();
	if (AnimInstance && MormalATKLeft && !AnimInstance->IsAnyMontagePlaying())
	{
		FOnMontageEnded EndDelegate;
		AnimInstance->Montage_Play(MormalATKLeft, 0.5f);

		// 몽타주 재생 종료 바인딩
		EndDelegate.BindUObject(this, &AMMGrux::EndLeftNormalATK);
		
		// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MormalATKLeft);
	}
}
void AMMGrux::RightHook()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopMovementImmediately();
	if (AnimInstance && MormalATKRight && !AnimInstance->IsAnyMontagePlaying())
	{
		FOnMontageEnded EndDelegate;
		AnimInstance->Montage_Play(MormalATKRight, 0.5f);

		// 몽타주 재생 종료 바인딩
		EndDelegate.BindUObject(this, &AMMGrux::EndRightNormalATK);
		
		// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MormalATKRight);
	}
}

bool AMMGrux::MontagePlaying()
{
	// 현재 몽타주가 실행 중인지 정보를 받기 위한 함수
	return GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
}

void AMMGrux::EndLeftNormalATK(class UAnimMontage* Montage, bool IsEnded)
{
	bNormalRightATK = true;
}

void AMMGrux::EndRightNormalATK(UAnimMontage* Montage, bool IsEnded)
{
	bNormalRightATK = false;
}

void AMMGrux::ATKChecking()
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

void AMMGrux::AngryNormalATKLeft()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopMovementImmediately();
	if (AnimInstance && AngryATKLeft && !AnimInstance->IsAnyMontagePlaying())
	{
		FOnMontageEnded EndDelegate;
		AnimInstance->Montage_Play(AngryATKLeft, 1.0f);

		// 몽타주 재생 종료 바인딩
		EndDelegate.BindUObject(this, &AMMGrux::EndLeftNormalATK);

		// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AngryATKLeft);
	}
}
void AMMGrux::AngryNormalATKRight()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopMovementImmediately();
	if (AnimInstance && AngryATKRight && !AnimInstance->IsAnyMontagePlaying())
	{
		FOnMontageEnded EndDelegate;
		AnimInstance->Montage_Play(AngryATKRight, 1.0f);

		// 몽타주 재생 종료 바인딩
		EndDelegate.BindUObject(this, &AMMGrux::EndRightNormalATK);

		// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AngryATKRight);
	}
}

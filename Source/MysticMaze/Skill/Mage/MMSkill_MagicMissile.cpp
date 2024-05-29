// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Mage/MMSkill_MagicMissile.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Item/MMEnergyBall.h"
#include "Collision/MMCollision.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

UMMSkill_MagicMissile::UMMSkill_MagicMissile()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Mage/AM_MagicMissile.AM_MagicMissile'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FClassFinder<AActor>EnergyBallRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Items/Weapons/BP_MagicMissile.BP_MagicMissile_C'"));
	if (EnergyBallRef.Succeeded())
	{
		EnergyBallClass = EnergyBallRef.Class;
	}

	EnergyBallCount = 0;
}

bool UMMSkill_MagicMissile::UseSkill()
{
	bool Result = Super::UseSkill();

	if (!Result) return false;

	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 플레이어 이동 막기
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// 애님 몽타주 재생
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(SkillMontage);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UMMSkill_MagicMissile::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);

		// EnergyBalls 초기화
		EnergyBalls.Reset();
	}

	return Result;
}

void UMMSkill_MagicMissile::SkillAttack()
{
	Super::SkillAttack();

	// 플레이어 전방의 가장 가까운 타겟 찾기
	TArray<FHitResult> HitResults;
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * 2000;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Shoot), false, Owner);

	// 충돌 탐지
	bool bHasHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(100),
		Params
	);

	AActor* NearestActor = nullptr;

	// 충돌이 존재한다면?
	if (bHasHit)
	{
		// 가장 가까운 적을 찾기
		for (const FHitResult& HitResult : HitResults)
		{
			float MinDistance = 10000.0f;

			float DistanceToMonster = FVector::Dist(Owner->GetActorLocation(), HitResult.GetActor()->GetActorLocation());
			if (DistanceToMonster < MinDistance)
			{
				NearestActor = HitResult.GetActor();
				MinDistance = DistanceToMonster;
			}
		};
	}
	
	for (int i = 0; i < 3; i++)
	{
		// 스폰 위치 지정
		FTransform Transform;
		Transform.SetLocation(Owner->GetActorLocation() + Owner->GetActorUpVector() * 200.0f);
		Transform.SetRotation(Owner->GetActorRotation().Quaternion());
		Transform.SetScale3D(FVector(0.5f));

		// 에너지볼 생성
		AMMEnergyBall* EnergyBall = GetWorld()->SpawnActor<AMMEnergyBall>(EnergyBallClass, Transform);
		EnergyBall->SetOwner(Owner);

		// 데미지 설정
		EnergyBall->SetDamage(BaseDamage);
		// NearestActor가 유효하면 타겟으로 삼기
		if (NearestActor)
		{
			EnergyBall->SetTarget(NearestActor);
		}

		// 스폰된 에너지볼 저장
		EnergyBalls.Add(EnergyBall);
	}
}

void UMMSkill_MagicMissile::Cancel()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 스킬 몽타주 재생 종료하기
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// 타이머 초기화
		EnergyBallTimer.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(EnergyBallTimer, this, &UMMSkill_MagicMissile::FireEnergyBall, 0.5f, true, 0.1f);

		// 스킬 종료 알림
		IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
		if (Owner)
		{
			SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
		}
	}
}

void UMMSkill_MagicMissile::SkillEnd(UAnimMontage* Montage, bool IsEnded)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	EnergyBallTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(EnergyBallTimer, this, &UMMSkill_MagicMissile::FireEnergyBall, 0.5f, true, 0.1f);

	// 스킬 종료 알림
	IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
	if (Owner)
	{
		SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
	}
}

void UMMSkill_MagicMissile::FireEnergyBall()
{
	if (EnergyBallCount < 3)
	{
		if (EnergyBalls.IsValidIndex(EnergyBallCount))
		{
			EnergyBalls[EnergyBallCount]->Fire();
			EnergyBalls[EnergyBallCount]->SetLifeSpan(2.0f);
			EnergyBallCount++;
		}
	}
	else
	{
		EnergyBallCount = 0;
		GetWorld()->GetTimerManager().PauseTimer(EnergyBallTimer);
	}
}

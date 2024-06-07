// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Mage/MMSkill_Flamethrower.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "MMSkill_Flamethrower.h"
#include "Collision/MMCollision.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UMMSkill_Flamethrower::UMMSkill_Flamethrower()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Mage/AM_Flamethrower.AM_Flamethrower'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UNiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Niagara/NiagaraBasic/07-SubUVAnimation/NS_FlameThrower.NS_FlameThrower'"));
	if (UNiagaraRef.Object)
	{
		NiagaraEffect = UNiagaraRef.Object;
	}
}

bool UMMSkill_Flamethrower::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_Flamethrower::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_Flamethrower::SkillAttack()
{
	Super::SkillAttack();

	if (NiagaraEffect)
	{
		FVector SpawnLotation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 130.0f + Owner->GetActorUpVector() * 30.0f;
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, SpawnLotation, Owner->GetActorRotation());
	}

	// 타이머 초기화
	AttackCheckTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(AttackCheckTimer, this, &UMMSkill_Flamethrower::AttackCheck, 0.3f, true, 0.1f);
}

void UMMSkill_Flamethrower::SkillAttackEnd()
{
	Super::SkillAttackEnd();

	if (NiagaraComponent)
		NiagaraComponent->Deactivate();
	GetWorld()->GetTimerManager().PauseTimer(AttackCheckTimer);
}

void UMMSkill_Flamethrower::Cancel()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 스킬 몽타주 재생 종료하기
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// 스킬 종료 알림
		IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
		if (Owner)
		{
			SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
		}

		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// 스킬 이펙트 및 타이머 종료
		SkillAttackEnd();
	}
}

void UMMSkill_Flamethrower::SkillEnd(UAnimMontage* Montage, bool IsEnded)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	// 스킬 종료 알림
	IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
	if (Owner)
	{
		SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
	}
}

void UMMSkill_Flamethrower::AttackCheck()
{
	// 플레이어 전방으로 공격 체크
	TArray<FHitResult> HitResults;
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * 600;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Shoot), false, Owner);

	// 충돌 탐지
	bool bHasHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(150),
		Params
	);

	// 충돌이 존재한다면?
	if (bHasHit)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (!PlayerCharacter) return;

		// 모든 적에게 데미지 전달
		for (const FHitResult& HitResult : HitResults)
		{
			// 데미지 전달
			if (Cast<ACharacter>(HitResult.GetActor()))
			{
				// 치명타 체크하기
				bool Critical = FMath::FRand() < (CriticalRate / 100);

				UGameplayStatics::ApplyDamage(HitResult.GetActor(), Critical ? BaseDamage * 2.0f : BaseDamage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
			}
		}
	}
}

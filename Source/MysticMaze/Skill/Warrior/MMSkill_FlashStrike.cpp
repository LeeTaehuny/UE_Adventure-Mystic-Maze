// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Warrior/MMSkill_FlashStrike.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Collision/MMCollision.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UMMSkill_FlashStrike::UMMSkill_FlashStrike()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Warrior/AM_FlashStrike.AM_FlashStrike'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UNiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Vampire.NS_Dash_Vampire'"));
	if (UNiagaraRef.Object)
	{
		NiagaraEffect = UNiagaraRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit1.P_ky_hit1'"));
	if (HitEffectRef.Object)
	{
		HitEffect = HitEffectRef.Object;
	}

	StepDistance = 900.0f;
}

bool UMMSkill_FlashStrike::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_FlashStrike::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	// 배열 초기화
	HitMonsters.Reset();

	return Result;
}

void UMMSkill_FlashStrike::SkillAttack()
{
	Super::SkillAttack();

	// 이펙트 재생하기
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, Owner->GetActorLocation(), Owner->GetActorRotation());

	// 이동 위치 탐색하기
	TArray<FHitResult> Results;
	FVector Start = Owner->GetActorLocation() ;
	FVector End = Start + (Owner->GetActorForwardVector() * StepDistance);
	FCollisionQueryParams Params;

	// 이동 범위 몬스터 탐색하기
	TArray<FHitResult> MonsterResults;

	TArray<AActor*> Ignores;
	Ignores.Add(Owner);

	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	bool bHasHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(), Start, End, FVector(0.0f, 100.0f, 90.0f), Owner->GetActorRotation(), Objects, false, Ignores, EDrawDebugTrace::ForDuration, MonsterResults, true);

	if (bHasHit)
	{
		// 충돌한 캐릭터 순회
		for (FHitResult Monster : MonsterResults)
		{
			// 충돌한 액터가 플레이어라면 건너뛰기
			if (Monster.GetActor() == Owner) continue;

			// 몬스터 배열에 추가
			HitMonsters.Add(Monster.GetActor());
		}

		// 타이머 초기화 및 매핑
		AttackTimer.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &UMMSkill_FlashStrike::Attack, 1.0f, false);
	}

	bool bHasMoveHit = GetWorld()->SweepMultiByChannel(
		Results,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_VISIBILITY,
		FCollisionShape::MakeSphere(40.0f),
		Params
	);

	// 최종 텔레포트 위치
	FVector TeleportLocation = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * StepDistance);

	// 충돌이 있었다면?
	if (bHasMoveHit)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (!PlayerCharacter) return;

		FVector MaxDistance = Owner->GetActorLocation();
		bool IsRun = false;

		for (FHitResult Result : Results)
		{
			// 몬스터가 아니라면?
			if (!Cast<ACharacter>(Result.GetActor()))
			{
				// 충돌한 액터까지의 거리가 기존 텔레포트 거리보다 길다면?
				if ((MaxDistance - Owner->GetActorLocation()).Length() < (Result.ImpactPoint - Owner->GetActorLocation()).Length())
				{
					// 최종 위치 반영
					MaxDistance = Result.ImpactPoint;
					IsRun = true;
				}
			}
		}

		// 반영된 위치가 존재한다면?
		if (IsRun)
		{
			// 반영된 위치로 플레이어 순간이동 시키기
			Owner->TeleportTo(MaxDistance, Owner->GetActorRotation(), false, true);
		}
		// 반영된 위치가 존재하지 않는다면?
		else
		{
			// 기존 위치로 플레이어 순간이동 시키기
			Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), false, true);
		}
	}
	else
	{
		// 플레이어 순간이동 시키기
		Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), false, true);
	}
}

void UMMSkill_FlashStrike::Cancel()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 스킬 몽타주 재생 종료하기
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// 스킬 종료 알림
		IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
		if (Owner)
		{
			SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
		}
	}
}

void UMMSkill_FlashStrike::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

void UMMSkill_FlashStrike::Attack()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (!PlayerCharacter) return;

	// 충돌했던 몬스터들을 순회하며 데미지 및 이펙트 발생시키기
	for (const auto& Monster : HitMonsters)
	{
		// 치명타 체크하기
		bool Critical = FMath::FRand() < (CriticalRate / 100);
		// 데미지 주기
		UGameplayStatics::ApplyDamage(Monster, Critical ? BaseDamage * 2.0f : BaseDamage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
		// 히트 이펙트 출력하기
		FTransform Transform;
		Transform.SetLocation(Monster->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Transform);
	}
}

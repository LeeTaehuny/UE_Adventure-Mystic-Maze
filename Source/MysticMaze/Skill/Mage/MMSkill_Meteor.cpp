// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Mage/MMSkill_Meteor.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Collision/MMCollision.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UMMSkill_Meteor::UMMSkill_Meteor()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Mage/AM_Meteor.AM_Meteor'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UNiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Niagara/NiagaraSkills/Meteor/NS_Meteor.NS_Meteor'"));
	if (UNiagaraRef.Object)
	{
		NiagaraEffect = UNiagaraRef.Object;
	}
}

bool UMMSkill_Meteor::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_Meteor::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_Meteor::SkillAttack()
{
	Super::SkillAttack();

	if (NiagaraEffect)
	{
		FHitResult HitResult;
		
		// 시작 지점 (플레이어 전방 1000 위치 위로 1000 위치)
		FVector Start = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 1000.0f + Owner->GetActorUpVector() * 1000.0f;
		// 종료 지점 (시작지점에서 아래로 2000위치)
		FVector End = Start + Owner->GetActorUpVector() * -2000.0f;
		// 파라미터
		FCollisionQueryParams Params;

		// 충돌 지점 구하기
		bool bHasHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			CHANNEL_VISIBILITY,
			Params
			);

		// 기본 스폰 장소
		SpawnPoint = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 1000.0f + Owner->GetActorUpVector() - 90.0f;

		// 바닥이 존재할 경우 스폰 장소
		if (bHasHit)
		{
			// 충돌 지점에 메테오 스폰
			SpawnPoint = HitResult.ImpactPoint;
		}

		// 메테오 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, SpawnPoint, Owner->GetActorRotation());
	}

	// 타이머 초기화
	AttackCheckTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(AttackCheckTimer, this, &UMMSkill_Meteor::AttackCheck, 0.5f, false);
}

void UMMSkill_Meteor::Cancel()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 스킬 몽타주 재생 종료하기
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// 스킬 종료
		SkillAttackEnd();
	}
}

void UMMSkill_Meteor::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

void UMMSkill_Meteor::AttackCheck()
{
	TArray<FOverlapResult> OverlapResults;

	float AttackRadius = 700.0f;
	FCollisionQueryParams Params;

	// 메테오 스폰 지점으로부터 반지름 700의 범위 체크
	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		SpawnPoint,
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 충돌한 경우
	if (bHasHit)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (!PlayerCharacter) return;

		// 플레이어를 제외한 나머지에게 데미지 전달
		for (FOverlapResult Result : OverlapResults)
		{
			// 플레이어의 경우 건너 뛰기
			if (Result.GetActor() == Owner) continue;

			// 데미지 전달
			if (Cast<ACharacter>(Result.GetActor()))
			{
				// 치명타 체크하기
				bool Critical = FMath::FRand() < (CriticalRate / 100);

				UGameplayStatics::ApplyDamage(Result.GetActor(), Critical ? BaseDamage * 2.0f : BaseDamage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
			}
		}
	}
}

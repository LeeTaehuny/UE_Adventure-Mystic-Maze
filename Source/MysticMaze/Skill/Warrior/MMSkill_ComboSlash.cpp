// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Warrior/MMSkill_ComboSlash.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Collision/MMCollision.h"

#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UMMSkill_ComboSlash::UMMSkill_ComboSlash()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Warrior/AM_ComboSlashSkill.AM_ComboSlashSkill'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}
}

bool UMMSkill_ComboSlash::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_ComboSlash::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_ComboSlash::SkillAttackCheck()
{
	Super::SkillAttackCheck();

	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		// 충돌 결과를 반환하기 위한 배열
		TArray<FHitResult> OutHitResults;

		// 공격 반경
		float AttackRange = 150.0f * SkillLevel;
		// 공격 체크를 위한 구체의 반지름
		float AttackRadius = 50.0f * SkillLevel;

		// 충돌 탐지를 위한 시작 지점 (플레이어 현재 위치 + 전방 방향 플레이어의 CapsuleComponent의 반지름 거리)
		FVector Start = PlayerCharacter->GetActorLocation() + (PlayerCharacter->GetActorForwardVector() * PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
		// 충돌 탐지 종료 지점 (시작지점 + 전방 방향의 공격 거리)
		FVector End = Start + (PlayerCharacter->GetActorForwardVector() * AttackRange);
		// 파라미터 설정
		FCollisionQueryParams Params;

		bool bHasHit = GetWorld()->SweepMultiByChannel(
			OutHitResults,
			Start,
			End,
			FQuat::Identity,
			CHANNEL_MMACTION,
			FCollisionShape::MakeSphere(AttackRadius),
			Params
		);

		if (bHasHit)
		{
			// TODO : 데미지 전달
			for (FHitResult Result : OutHitResults)
			{
				if (Result.GetActor() == Owner) continue;
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.GetActor()->GetName());
			}
		}

		// Capsule 모양의 디버깅 체크
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(PlayerCharacter->GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
	}
}

void UMMSkill_ComboSlash::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

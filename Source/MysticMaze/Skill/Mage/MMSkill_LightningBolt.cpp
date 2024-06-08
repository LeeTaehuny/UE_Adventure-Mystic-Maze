// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Mage/MMSkill_LightningBolt.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UMMSkill_LightningBolt::UMMSkill_LightningBolt()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Mage/AM_LightningBolt.AM_LightningBolt'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> UParticleSystemRef(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_lightning3.P_ky_lightning3'"));
	if (UParticleSystemRef.Object)
	{
		Effect = UParticleSystemRef.Object;
	}
}

bool UMMSkill_LightningBolt::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_LightningBolt::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_LightningBolt::SkillAttack()
{
	TArray<FOverlapResult> OverlapResults;

	float AttackRadius = 1000.0f;
	FCollisionQueryParams Params;

	// 플레이어 중심으로부터 1000의 범위 체크
	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Owner->GetActorLocation(),
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

			// 파티클 스폰
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, Result.GetActor()->GetTransform());

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

void UMMSkill_LightningBolt::Cancel()
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

void UMMSkill_LightningBolt::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

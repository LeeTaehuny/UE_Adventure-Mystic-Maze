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
		// �÷��̾� �̵� ����
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// �ִ� ��Ÿ�� ���
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(SkillMontage);

		// ��Ÿ�� ��� ���� ���ε�
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

	// �÷��̾� �߽����κ��� 1000�� ���� üũ
	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Owner->GetActorLocation(),
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// �浹�� ���
	if (bHasHit)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (!PlayerCharacter) return;

		// �÷��̾ ������ ���������� ������ ����
		for (FOverlapResult Result : OverlapResults)
		{
			// �÷��̾��� ��� �ǳ� �ٱ�
			if (Result.GetActor() == Owner) continue;

			// ��ƼŬ ����
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, Result.GetActor()->GetTransform());

			// ������ ����
			if (Cast<ACharacter>(Result.GetActor()))
			{
				// ġ��Ÿ üũ�ϱ�
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
		// ��ų ��Ÿ�� ��� �����ϱ�
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// ��ų ����
		SkillAttackEnd();
	}
}

void UMMSkill_LightningBolt::SkillEnd(UAnimMontage* Montage, bool IsEnded)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		// �÷��̾� �̵� �����ϰ� ����
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	// ��ų ���� �˸�
	IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
	if (Owner)
	{
		SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
	}
}

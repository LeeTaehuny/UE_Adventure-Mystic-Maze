// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Archer/MMSkill_WindStorm.h"
#include "Skill/MMSkillData.h"
#include "Skill/MMStome.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UMMSkill_WindStorm::UMMSkill_WindStorm()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Archer/AM_WindStorm.AM_WindStorm'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}
}

bool UMMSkill_WindStorm::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_WindStorm::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_WindStorm::SkillAttack()
{
	Super::SkillAttack();

	// 토네이도 스폰 지점 구하기
	FTransform Transform;
	Transform.SetLocation(Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 1000.0f) + (Owner->GetActorUpVector() * -45.0f));
	Transform.SetRotation(Owner->GetActorRotation().Quaternion());
	Transform.SetScale3D(FVector(3.0f));

	// 토네이도 스폰
	AMMStome* Storm = GetWorld()->SpawnActorDeferred<AMMStome>(AMMStome::StaticClass(), Transform);
	// 시작 전 설정
	Storm->SetOwner(Owner);
	Storm->SetDamage(BaseDamage);
	Storm->SetCriticalRate(CriticalRate);
	// 스폰 완료
	Storm->FinishSpawning(Transform);
}

void UMMSkill_WindStorm::Cancel()
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

void UMMSkill_WindStorm::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

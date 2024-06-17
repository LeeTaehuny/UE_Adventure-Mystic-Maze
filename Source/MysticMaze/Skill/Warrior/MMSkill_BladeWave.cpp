// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Warrior/MMSkill_BladeWave.h"
#include "Skill/MMSkillData.h"
#include "Skill/MMBladeWave.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMMSkill_BladeWave::UMMSkill_BladeWave()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Warrior/AM_BladeWave.AM_BladeWave'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}
}

bool UMMSkill_BladeWave::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_BladeWave::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_BladeWave::SkillAttack()
{
	Super::SkillAttack();

	// 스폰 지점 구하기
	// * 플레이어 위치와 방향 받아오기
	FVector PlayerLocation = Owner->GetActorLocation();
	FRotator PlayerRotation = Owner->GetActorRotation();
	
	// * 전방, 양측으로 15도 회전된 방향 구하기
	FRotator PlayerRotationLeft = UKismetMathLibrary::ComposeRotators(PlayerRotation, FRotator(0.0f, -15.0f, 0.0f));
	FRotator PlayerRotationRight = UKismetMathLibrary::ComposeRotators(PlayerRotation, FRotator(0.0f, 15.0f, 0.0f));

	// 스폰하기
	FTransform Transform;
	Transform.SetLocation(PlayerLocation);
	Transform.SetRotation(PlayerRotation.Quaternion());
	AMMBladeWave* Wave1 = GetWorld()->SpawnActorDeferred<AMMBladeWave>(AMMBladeWave::StaticClass(), Transform);
	// 시작 전 설정
	Wave1->SetOwner(Owner);
	Wave1->SetDamage(BaseDamage);
	Wave1->SetCriticalRate(CriticalRate);
	// 스폰 완료
	Wave1->FinishSpawning(Transform);

	Transform.SetRotation(PlayerRotationLeft.Quaternion());
	AMMBladeWave* Wave2 = GetWorld()->SpawnActorDeferred<AMMBladeWave>(AMMBladeWave::StaticClass(), Transform);
	// 시작 전 설정
	Wave2->SetOwner(Owner);
	Wave2->SetDamage(BaseDamage);
	Wave2->SetCriticalRate(CriticalRate);
	// 스폰 완료
	Wave2->FinishSpawning(Transform);

	Transform.SetRotation(PlayerRotationRight.Quaternion());
	AMMBladeWave* Wave3 = GetWorld()->SpawnActorDeferred<AMMBladeWave>(AMMBladeWave::StaticClass(), Transform);
	// 시작 전 설정
	Wave3->SetOwner(Owner);
	Wave3->SetDamage(BaseDamage);
	Wave3->SetCriticalRate(CriticalRate);
	// 스폰 완료
	Wave3->FinishSpawning(Transform);
}

void UMMSkill_BladeWave::Cancel()
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

void UMMSkill_BladeWave::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

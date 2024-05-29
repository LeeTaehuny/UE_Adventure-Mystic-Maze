// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Archer/MMSkill_Sniping.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

UMMSkill_Sniping::UMMSkill_Sniping()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Archer/AM_Sniping.AM_Sniping'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

}

bool UMMSkill_Sniping::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_Sniping::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_Sniping::SkillAttack()
{
	Super::SkillAttack();
}

void UMMSkill_Sniping::Cancel()
{
	Super::Cancel();
}

void UMMSkill_Sniping::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

void UMMSkill_Sniping::FireArrow()
{
}

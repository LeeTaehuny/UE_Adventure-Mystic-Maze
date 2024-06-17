// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/MMSkillBase.h"
#include "Skill/MMSkillData.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"

#include "GameFramework/Character.h"

UMMSkillBase::UMMSkillBase()
{
	// 쿨타임 진행중 X
	bIsCoolDown = false;
}

void UMMSkillBase::Init(UMMSkillData* InSkillData, int32 InSkillLevel, AActor* InOwner)
{
	if (IsValid(InSkillData) && IsValid(InOwner))
	{
		SkillData = InSkillData;
		SkillLevel = InSkillLevel;
		Owner = InOwner;

		CurrentCoolTime = InSkillData->CoolTime;
	}
}

bool UMMSkillBase::UseSkill()
{
	// 쿨타임이 돌고 있으면 반환
	if (bIsCoolDown) return false;

	// 마나 확인하기
	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(Owner);
	if (StatPlayer && StatPlayer->GetStatComponent()->GetCurrentMp() >= SkillData->ManaCost)
	{
		// 마나 감소
		StatPlayer->GetStatComponent()->UseMp(SkillData->ManaCost);
		// 데미지 저장
		BaseDamage = StatPlayer->GetStatComponent()->GetAttackDamage() * SkillData->Multiplier[SkillLevel - 1];
		// 치명타 확률 저장
		CriticalRate = StatPlayer->GetStatComponent()->GetCriticalHitRate();

		// 쿨타임이 돌도록 설정
		bIsCoolDown = true;

		// 바라보는 방향 전환하기
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (PlayerCharacter)
		{
			PlayerCharacter->SetActorRotation(FRotator(PlayerCharacter->GetActorRotation().Pitch, PlayerCharacter->GetControlRotation().Yaw, PlayerCharacter->GetActorRotation().Roll));
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

bool UMMSkillBase::CoolDown(float DeltaSeconds)
{
	// 현재 쿨타임이 돌고 있으면?
	if (bIsCoolDown)
	{
		// 쿨타임 시간 감소시키기
		CurrentCoolTime = FMath::Clamp(CurrentCoolTime - DeltaSeconds, 0.0f, SkillData->CoolTime);

		// 쿨타임이 0이 되면 스킬 사용 가능하도록 설정하기
		if (CurrentCoolTime == 0.0f)
		{
			bIsCoolDown = false;
			CurrentCoolTime = SkillData->CoolTime;
		}

		return true;
	}
	else
	{
		return false;
	}
}

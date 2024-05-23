// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/MMSkillBase.h"
#include "Skill/MMSkillData.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"

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
	IMMStatusInterface* SkillPlayer = Cast<IMMStatusInterface>(Owner);
	if (SkillPlayer && SkillPlayer->GetStatComponent()->GetCurrentMp() >= SkillData->ManaCost)
	{
		// 마나 감소
		SkillPlayer->GetStatComponent()->UseMp(SkillData->ManaCost);

		// 쿨타임이 돌도록 설정
		bIsCoolDown = true;

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

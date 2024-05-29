// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MMSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UMMSkillBase();

public:
	void Init(class UMMSkillData* InSkillData, int32 InSkillLevel, AActor* InOwner);

public:
	FORCEINLINE class UMMSkillData* GetSkillData() { return SkillData; }
	FORCEINLINE float GetCurrentCoolTime() { return CurrentCoolTime; }
	FORCEINLINE int32 GetSkillLevel() { return SkillLevel; }
	FORCEINLINE void SetSkillLevel(int32 InLevel) { SkillLevel = InLevel; }

	virtual bool UseSkill();
	virtual void SetTarget(AActor* Target) { }
	bool CoolDown(float DeltaSeconds);
	virtual void SkillAttack() {}
	virtual void SkillAttackEnd() {}
	virtual void Cancel() {}

protected:
	UPROPERTY(VisibleAnywhere, Category = SkillBase, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMMSkillData> SkillData;

	// 스킬 레벨
	UPROPERTY(VisibleAnywhere, Category = SkillBase, meta = (AllowPrivateAccess = true))
	int32 SkillLevel;

protected:
	TObjectPtr<AActor> Owner;

	// 쿨타임 진행중인지 판별하기 위한 변수
	uint8 bIsCoolDown;

	// 스킬의 기본 데미지 저장
	float BaseDamage;
	// 치명타 확률 저장
	float CriticalRate;

private:
	// 현재 스킬의 쿨타임
	float CurrentCoolTime;
};

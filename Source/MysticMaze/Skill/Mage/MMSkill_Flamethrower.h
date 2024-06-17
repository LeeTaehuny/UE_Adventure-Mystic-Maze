// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/MMSkillBase.h"
#include "MMSkill_Flamethrower.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkill_Flamethrower : public UMMSkillBase
{
	GENERATED_BODY()
	
public:
	UMMSkill_Flamethrower();

public:
	virtual bool UseSkill() override;
	virtual void SkillAttack() override;
	virtual void SkillAttackEnd() override;
	virtual void Cancel() override;

private:
	void SkillEnd(class UAnimMontage* Montage, bool IsEnded);

	UPROPERTY(VisibleAnywhere, Category = "Details")
	TObjectPtr<class UAnimMontage> SkillMontage;

	// 화염방사 공격 체크에 필요한 타이머 변수와 함수
	FTimerHandle AttackCheckTimer;
	void AttackCheck();

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> NiagaraEffect;
};

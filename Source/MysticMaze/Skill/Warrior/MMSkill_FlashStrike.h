// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/MMSkillBase.h"
#include "MMSkill_FlashStrike.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkill_FlashStrike : public UMMSkillBase
{
	GENERATED_BODY()
	
public:
	UMMSkill_FlashStrike();

public:
	virtual bool UseSkill() override;
	virtual void SkillAttack() override;
	virtual void Cancel() override;

private:
	void SkillEnd(class UAnimMontage* Montage, bool IsEnded);
	void Attack();

	UPROPERTY(VisibleAnywhere, Category = "Details")
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> NiagaraEffect;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> HitEffect;

	TArray<TObjectPtr<AActor>> HitMonsters;

	float StepDistance;
	FTimerHandle AttackTimer;
};

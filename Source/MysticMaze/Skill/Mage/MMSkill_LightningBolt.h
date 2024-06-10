// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/MMSkillBase.h"
#include "MMSkill_LightningBolt.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkill_LightningBolt : public UMMSkillBase
{
	GENERATED_BODY()
	
public:
	UMMSkill_LightningBolt();

public:
	virtual bool UseSkill() override;
	virtual void SkillAttack() override;
	virtual void Cancel() override;

private:
	void SkillEnd(class UAnimMontage* Montage, bool IsEnded);

	UPROPERTY(VisibleAnywhere, Category = "Details")
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> Effect;
};

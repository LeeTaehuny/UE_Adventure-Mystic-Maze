// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/MMSkillBase.h"
#include "MMSkill_MultiShot.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkill_MultiShot : public UMMSkillBase
{
	GENERATED_BODY()
	
public:
	UMMSkill_MultiShot();

public:
	virtual bool UseSkill() override;
	virtual void SkillAttack() override;
	virtual void Cancel() override;

private:
	void SkillEnd(class UAnimMontage* Montage, bool IsEnded);

	UPROPERTY(VisibleAnywhere, Category = "Details")
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TSubclassOf<AActor> ArrowClass;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class AMMArrow>> Arrows;

	TArray<FVector> Positions;

	FTimerHandle ArrowFireTimer;

	void FireArrow();
};

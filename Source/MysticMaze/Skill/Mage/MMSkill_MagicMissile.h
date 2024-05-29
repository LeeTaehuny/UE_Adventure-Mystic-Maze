// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/MMSkillBase.h"
#include "MMSkill_MagicMissile.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMSkill_MagicMissile : public UMMSkillBase
{
	GENERATED_BODY()
	
public:
	UMMSkill_MagicMissile();

public:
	virtual bool UseSkill() override;
	virtual void SkillAttack() override;
	virtual void Cancel() override;

private:
	void SkillEnd(class UAnimMontage* Montage, bool IsEnded);

	UPROPERTY(VisibleAnywhere, Category = "Details")
	TObjectPtr<class UAnimMontage> SkillMontage;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TArray<TObjectPtr<class AMMEnergyBall>> EnergyBalls;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TSubclassOf<AActor> EnergyBallClass;

	// 에너지볼 발사에 필요한 타이머 변수와 함수
	FTimerHandle EnergyBallTimer;
	int32 EnergyBallCount;
	void FireEnergyBall();
};

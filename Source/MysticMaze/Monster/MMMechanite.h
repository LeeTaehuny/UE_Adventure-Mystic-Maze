﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMMechaniteAnimInterface.h"
#include "Interface/MMMonsterATKChekInterface.h"
#include "Interface/MMMechaniteRushATKInterface.h"
#include "Interface/MMMonsterATKModeInterface.h"		// 공격 모드 변수의 값을 전달하기 위함
#include "Interface/MMMechaniteAIInterface.h"			// ai 컨트롤러에 값을 전달하기 위한 인터페이스

#include "MMMechanite.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMMechanite : public AMMMonsterBase, 
	public IMMMechaniteAnimInterface, 
	public IMMMonsterATKChekInterface,
	public IMMMechaniteRushATKInterface,
	public IMMMonsterATKModeInterface,
	public IMMMechaniteAIInterface
{
	GENERATED_BODY()
	
public:
	AMMMechanite();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RushATK_End(class UAnimMontage* Montage, bool IsEnded);

	// IMMMonsterATKChekInterface : 공격을 감지하는 인터페이스
	virtual void ATKChecking() override;

private:

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ATKNormal;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ATKRush;

	// IMMMechaniteAnimInterface : 설정된 애니메이션을 시작 시키기 위한 인터페이스
	virtual void StartMechaniteAnim(int INChoiceMontage) override;
	virtual void EndMechaniteAnim() override;
	virtual void AnimEnd(bool INData) override;
	virtual void SettingMoveSpeed(float INDats) override;
	virtual void MovingStop() override;
	virtual bool GetCoolOn() override { return RushATKCoolOn; }
	virtual void SetCoolOn(bool InData) { RushATKCoolOn = InData; }
	virtual void RushATKCheck() override;

	// IMMMechaniteRushATKInterface : 러쉬 공격 전용 인터페이스
	virtual void ChangeCollision() override;

	// IMMMonsterATKModeInterface : 공격 모드 변수의 값을 전달하기 위한 함수
	FORCEINLINE virtual bool GetATKMode() override { return ATK_Mode; }
	FORCEINLINE virtual void SetATKMode(bool INData) override { ATK_Mode = INData; };

	// IMMMechaniteAIInterface : ai 컨트롤러에 값을 전달하기 위한 인터페이스

private:
	

private:
	uint8 RushMontageEnd : 1;

	float WalkSpeed = 300.0f;
	int AKTChoice = 0;

	float RushATKCoolDown = 0;
	float RushATKCoolDown_MaxTime = 0;
	uint8 RushATKCoolOn : 1;

};

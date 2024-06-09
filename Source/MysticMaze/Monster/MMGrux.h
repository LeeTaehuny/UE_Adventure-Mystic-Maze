// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMMonsterSpawnInterface.h"
#include "Interface/MMAnimationGruxInterface.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "Interface/MMMonsterATKModeInterface.h"
#include "Interface/MMGruxNormalATKInterface.h"			// 그룩스에게 공격 몽타주 실행을 명령하는 인터페이스
#include "Interface/MMGruxAIDataInterface.h"			// AI 컨트롤러에 값을 전달하기 위한 인터페이스
#include "Interface/MMGruxModeChangeInterface.h"		// 공격모드 체인지 노티파이 신호가 올 경우 작동
#include "Interface/MMGruxAngryNormalATKInterface.h"	// 분노 상태일때 노말 공격 몽타주를 실행하기 위한 인터페이스
#include "Interface/MMGruxModeChangeCPInterface.h"		// 몬스터 분노모드 변경 애니메이션이 끝났을때 실행되는 인터페이스
#include "Interface/MMMonsterDieInterface.h"			// 몬스터가 죽었을 때 정보를 주고 받기 위한 인터페이스
#include "Interface/MMGruxCoolDownInterface.h"			// 그룩스의 돌진 쿨타임을 관리하기 위한 인터페이스

#include "MMGrux.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGrux : public AMMMonsterBase, 
	public IMMMonsterSpawnInterface, 
	public IMMAnimationGruxInterface,
	public IMMBugSwarmAIDataInterface,
	public IMMMonsterATKModeInterface,
	public IMMGruxNormalATKInterface,
	public IMMGruxAIDataInterface,
	public IMMMonsterATKChekInterface,
	public IMMGruxModeChangeInterface,
	public IMMGruxAngryNormalATKInterface,
	public IMMGruxModeChangeCPInterface,
	public IMMMonsterDieInterface,
	public IMMGruxCoolDownInterface
{
	GENERATED_BODY()
	
public:
	AMMGrux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MonsterDieMontage() override;

	//IMMAnimationGruxInterface : 애니메이션 인터페이스 함수
	FORCEINLINE virtual bool GetDie() override { return bDie; }
	FORCEINLINE virtual bool GetSpawn() override { return bSpawn; }
	FORCEINLINE virtual bool GetDieMotion() override { return bRandomDieMotion; }
	FORCEINLINE virtual bool GetAngryModeChangeComplete() override { return bAngryModeChangeComplete; }

	// IMMBugSwarmAIDataInterface : ai 정보를 수정하기 위한 인터페이스 함수
	FORCEINLINE virtual bool GetATKMode() override { return ATK_Mode; };

	// IMMMonsterSpawnInterface : 노티파이 데이터를 받아오기 위한 함수
	FORCEINLINE virtual void SetSpawnData(bool INspawn) override { bSpawn = INspawn; }

	FORCEINLINE virtual void SetATKMode(bool INData) override { ATK_Mode = INData; };

	// IMMGruxModeChangeCPInterface : 모드 변경 노티파이가 실행됐을때 값을 변경하기 위한 함수
	FORCEINLINE virtual void SetMode(bool INData) { bAngryModeChangeComplete = INData; }

	// IMMMonsterDieInterface : 몬스터가 죽었을때 작동하는 함수
	virtual void Monsterdie() override;

	// IMMGruxCoolDownInterface : 그룩스의 돌진 쿨타임을 관리하기 위한 함수
	virtual float GetCoolDown() override { return RushCoolDown; }
	virtual void SetCoolDown(float INData) override { RushCoolDown = INData; }
	virtual bool GetCoolOn() { return RushCoolOn; }
	virtual void SetCoolOn(bool INData) override { RushCoolOn = INData; }
		
protected: // 그룩스의 애니메이션 몽타주 데이터를 저장하기 위한 변수 선언 및 몽타주 관련 함수

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MormalATKLeft;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MormalATKRight;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AngryATKLeft;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AngryATKRight;

	// IMMGruxNormalATKInterface : 일반공격 몽타주 실행 함수
	virtual void  LeftHook() override;
	virtual void  RightHook() override;
	virtual bool MontagePlaying() override;
	void EndLeftNormalATK(class UAnimMontage* Montage, bool IsEnded);
	void EndRightNormalATK(class UAnimMontage* Montage, bool IsEnded);

	// IMMGruxAIDataInterface : AI 컨트롤러에 값을 전달하기 위한 함수들
	FORCEINLINE virtual bool GetGruxNormalATK() override { return bNormalRightATK; }
	FORCEINLINE virtual bool GetAngryMode() override { return bAngryMode; }
	FORCEINLINE virtual float GetHPPercent() override { return HP_Percent; }

	// IMMMonsterATKChekInterface : 공격용 노티파이 (호출시 적을 공격할 수 있음)
	virtual void ATKChecking() override;

	// IMMGruxModeChangeInterface : 노티파이를 이용한 공격모드 변경 함수
	virtual void AngryModeChange(bool INData) override { bAngryMode = INData; }

	// IMMGruxAngryNormalATKInterface : 분노 상태일때 노말 공격 몽타주를 실행하기 위한 인터페이스
	virtual void AngryNormalATKLeft() override;
	virtual void AngryNormalATKRight() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bSpawn : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bRandomDieMotion : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bAngryModeChangeComplete : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	uint8 bAngryMode : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	uint8 bNormalRightATK : 1;

protected: // 그룩스 개인의 스킬 쿨타임
	
	float RushCoolDown = 0.0f;
	uint8 RushCoolOn : 1;

	

};

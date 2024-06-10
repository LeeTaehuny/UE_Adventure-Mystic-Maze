// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMAnimationBugSwarmAIInterface.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "Interface/MMMonsterSpawnInterface.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "Interface/MMMugSpeedATKInterface.h"			// 속도 공격을 위한 인터페이스
#include "Interface/MMMonsterATKModeInterface.h"		// 공격 모드 변수의 값을 전달하기 위함
#include "Interface/MMMonsterDieInterface.h"			// 몬스터가 죽었을 때 정보를 주고 받기 위한 인터페이스

#include "MonsterAI/AIController/MMBugSwarmAIController.h"
#include "MMBugSwarm.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMBugSwarm : public AMMMonsterBase, 
	public IMMAnimationBugSwarmAIInterface, 
	public IMMMonsterSpawnInterface, 
	public IMMBugSwarmAIDataInterface,
	public IMMMonsterATKChekInterface,
	public IMMMugSpeedATKInterface,
	public IMMMonsterATKModeInterface,
	public IMMMonsterDieInterface
{
	GENERATED_BODY()

public:
	AMMBugSwarm();	

	//void SetLeaderData(AActor* INPawn) { myLeader = INPawn; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MonsterDieMontage() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float FlyHeight;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bSpawn : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bRandomDieMotion : 1;

	//IMMAnimationBugSwarmAIInterface : 애니메이션 인터페이스 함수
	FORCEINLINE virtual bool GetDie() override { return bDie; }
	FORCEINLINE virtual bool GetSpawn() override { return bSpawn; }
	FORCEINLINE virtual bool GetDieMotion() override { return bRandomDieMotion; }

	// IMMMonsterSpawnInterface : 노티파이에서 스폰 값을 변경시키기 위한 인터페이스 함수들
	FORCEINLINE virtual void SetSpawnData(bool INspawn) override { bSpawn = INspawn; }

	// IMMBugSwarmAIDataInterface : AI에 정보를 전달하기 위한 인터페이스 함수
	FORCEINLINE virtual bool GetATKMode() override { return ATK_Mode; }
	//FORCEINLINE virtual bool GetFlying() override { return bBugFlying; }
	//FORCEINLINE virtual float GetZLocation() override { return ZLocation; }

	// IMMMonsterATKChekInterface : 공격을 감지하는 인터페이스
	virtual void ATKChecking() override;

	// IMMMugSpeedATKInterface : 단체 공격시 
	virtual void ATKOn() override;
	virtual void ATKOff() override;
	FORCEINLINE virtual float GetcurTime() override { return ATKing_Timer; }
	FORCEINLINE virtual void SetcurTime(float INDAta) override { ATKing_Timer = INDAta;	 }
	FORCEINLINE virtual float GetMaxTime() override { return MAX_ATKing_Timer; }

	// IMMMonsterATKModeInterface : 공격 모드 변수의 값을 전달하기 위한 함수
	FORCEINLINE virtual void SetATKMode(bool INData) override { ATK_Mode = INData; };

	// IMMMonsterDieInterface : 몬스터가 죽었을때 작동하는 함수
	virtual void Monsterdie() override;

	virtual void MonsterHitAnim() override;

protected: // AI 전용 변수들
	// 스폰지점을 저장하기 위한 변수
	//FVector CenterLocation;

	// 설정된 시간마다 지면 좌표 탐색을 하기 위한 변수
	float ATKing_Timer = 0.0f;
	float MAX_ATKing_Timer = 5.0f;
};

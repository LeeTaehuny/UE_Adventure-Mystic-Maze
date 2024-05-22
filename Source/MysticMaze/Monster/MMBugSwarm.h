// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMAnimationBugSwarmAIInterface.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "Interface/MMMonsterSpawnInterface.h"
#include "Interface/MMBugSwarmAIDataInterface.h"
#include "Interface/MMBugSwarmFlyingInterface.h"

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
	public IMMBugSwarmFlyingInterface
{
	GENERATED_BODY()

public:
	AMMBugSwarm();	

	//void SetLeaderData(AActor* INPawn) { myLeader = INPawn; }
	void SetCenterLocation(FVector InLocation) { CenterLocation = InLocation; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float FlyHeight;

protected: // 애니메이션 블루프린트용
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
	FORCEINLINE virtual FVector GetSpawnLocation() override { return CenterLocation; }
	FORCEINLINE virtual bool GetFlying() override { return bBugFlying; }
	FORCEINLINE virtual float GetZLocation() override { return ZLocation; }

	// IMMBugSwarmFlyingInterface : 비행 정보를 전달하기 위한 인터페이스 함수
	FORCEINLINE virtual void SetFlying(bool INData) override { bBugFlying = INData; };
	FORCEINLINE virtual void SetZLocation(float INData) override { ZLocation = INData; };
	FORCEINLINE virtual FVector SetMinZLocation(FVector INData) override { return MinZLocation; }


protected: // AI 전용 변수들
	// 스폰지점을 저장하기 위한 변수
	FVector CenterLocation;

	// 설정된 시간마다 지면 좌표 탐색을 하기 위한 변수
	float TickTime = 0;
	
	// 현재 비행중인지를 판별하기 위한 bool 변수
	uint8 bBugFlying : 1;
	// 밑 바닥의 좌표를 기록하기 위함
	float ZLocation = 1100;
	FVector MinZLocation = FVector();
};

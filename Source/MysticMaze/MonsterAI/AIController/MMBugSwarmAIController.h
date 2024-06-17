// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MMBugSwarmAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMBugSwarmAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMMBugSwarmAIController();

	FORCEINLINE UBehaviorTree* GetBehavior() { return BegAIBehavior; }

	UFUNCTION()
	void SettingBlackBoardData(bool INDie, bool INSpawn);

	FORCEINLINE void SetMyLeaderData(APawn* INLeader) { Myleader = INLeader; }
	FORCEINLINE void SetLeader(bool InData) { bLeaderItMe = InData; }

	// 함수에 버그스웜들의 정보를 저장하기 위한 함수
	void AddSoldier(AMMBugSwarmAIController* INSol) { Soldiers.Add(INSol); }

	void SetRehabilitation(bool INData) { bRehabilitation = INData; }

	TArray<AMMBugSwarmAIController*> GetSoldiersData() { return Soldiers; };

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	

protected:
	UPROPERTY(EditAnywhere, Category = "AI", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BegAIBehavior;


	// 리더의 정보를 저장하기 위한 변수
	APawn* Myleader;
	// 내가 리더인지 판별하기 위한 변수
	uint8 bLeaderItMe : 1;

	// 재정비 행동을 결정하는 변수
	uint8 bRehabilitation : 1;
	// 빠른공격 행동을 결정하는 변수
	uint8 bSpeedAttack : 1;
	// 기관총 행동을 결정하는 변수
	uint8 bMachineGun : 1;

	// 버그스웜들의 배열을 저장해놓기 위한 배열
	TArray<AMMBugSwarmAIController*> Soldiers;
	
};

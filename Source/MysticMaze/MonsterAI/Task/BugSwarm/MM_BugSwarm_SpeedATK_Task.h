// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "Navigation/PathFollowingComponent.h"
#include "MM_BugSwarm_SpeedATK_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_BugSwarm_SpeedATK_Task : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UMM_BugSwarm_SpeedATK_Task();

	// 버그스웜 행동
	// 속공을 실행하기 위한 테스크
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	uint8 justOne : 1;

	FVector HitLocation;

	AAIController* MyController;
	UCapsuleComponent* BugSwarmCapusulData;
};

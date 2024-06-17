// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MM_Mechanite_RushATK_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_Mechanite_RushATK_Task : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UMM_Mechanite_RushATK_Task();

	// 공격후, 다음 공격 행동을 정하기 위한 테스크
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	FVector  HitLocation;

};

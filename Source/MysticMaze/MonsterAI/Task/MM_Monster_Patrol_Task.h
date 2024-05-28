﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MM_Monster_Patrol_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_Monster_Patrol_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMM_Monster_Patrol_Task();

	// 공격후, 다음 공격 행동을 정하기 위한 테스크
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
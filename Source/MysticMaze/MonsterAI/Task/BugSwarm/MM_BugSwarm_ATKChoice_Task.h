// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MM_BugSwarm_ATKChoice_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_BugSwarm_ATKChoice_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMM_BugSwarm_ATKChoice_Task();

	// 공격후, 다음 공격 행동을 정하기 위한 테스크
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

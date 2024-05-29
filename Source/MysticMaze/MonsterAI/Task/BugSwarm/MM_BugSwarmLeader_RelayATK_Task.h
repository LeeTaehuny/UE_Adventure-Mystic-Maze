// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MonsterAI/AIController/MMBugSwarmAIController.h"
#include "MM_BugSwarmLeader_RelayATK_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_BugSwarmLeader_RelayATK_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMM_BugSwarmLeader_RelayATK_Task();

	// 버그스웜 행동
	// 리더가 버그스웜들을 정렬시키며 공격 명령을 내림
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	float TimerTicktok = 0;
	float DeleyTime = 0;
	int startLine = 1;
	int rightLine = 0;

	APawn* MyData;
	TArray<AMMBugSwarmAIController*> MopsData;
};

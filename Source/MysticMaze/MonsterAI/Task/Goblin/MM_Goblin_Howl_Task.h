// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MM_Goblin_Howl_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_Goblin_Howl_Task : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMM_Goblin_Howl_Task();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

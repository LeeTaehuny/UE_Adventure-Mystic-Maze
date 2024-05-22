// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MMFindLeaderTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMFindLeaderTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMMFindLeaderTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};

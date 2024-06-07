﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MM_Grux_AngryModeChange_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_Grux_AngryModeChange_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UMM_Grux_AngryModeChange_Task();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
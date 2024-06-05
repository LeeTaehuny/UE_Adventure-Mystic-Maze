// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MM_GobWizard_NormalATK_Task.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_GobWizard_NormalATK_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMM_GobWizard_NormalATK_Task();

	// ������, ���� ���� �ൿ�� ���ϱ� ���� �׽�ũ
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	TObjectPtr<APawn> PlayerData;

};

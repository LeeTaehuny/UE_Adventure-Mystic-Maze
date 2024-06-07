// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MM_Warrior_Protect_Service.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMM_Warrior_Protect_Service : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MMGoblinWarriorAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGoblinWarriorAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMMGoblinWarriorAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BegAIBehavior;
};

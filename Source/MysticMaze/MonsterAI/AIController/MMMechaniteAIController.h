// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MMMechaniteAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMMechaniteAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMMMechaniteAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "AI", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BegAIBehavior;

	TObjectPtr<UBlackboardComponent> MyBlackboardData;

};

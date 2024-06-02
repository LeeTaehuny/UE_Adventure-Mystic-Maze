// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MMGoblinWizardAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGoblinWizardAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMMGoblinWizardAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BegAIBehavior;
	
};

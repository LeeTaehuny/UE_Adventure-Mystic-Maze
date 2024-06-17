// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/MMBaseAnimInstance.h"
#include "MMGoblinWarriorAnim.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMGoblinWarriorAnim : public UMMBaseAnimInstance
{
	GENERATED_BODY()

public:
	UMMGoblinWarriorAnim();

	// Initialize
	virtual void NativeInitializeAnimation() override;

	// Update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bDie : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bRandomDieMotion : 1;
	
};

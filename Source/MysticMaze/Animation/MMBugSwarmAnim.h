// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/MMBaseAnimInstance.h"
#include "MMBugSwarmAnim.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMBugSwarmAnim : public UMMBaseAnimInstance
{
	GENERATED_BODY()

public:
	UMMBugSwarmAnim();

protected:
	// Initialize
	virtual void NativeInitializeAnimation() override;

	// Update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	
};

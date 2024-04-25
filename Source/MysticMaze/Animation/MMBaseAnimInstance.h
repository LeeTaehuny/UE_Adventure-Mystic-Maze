﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MMBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMMBaseAnimInstance();

protected:
	// Initialize
	virtual void NativeInitializeAnimation() override;

	// Update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
// Base Move
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsMove : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovingThreshould;
};

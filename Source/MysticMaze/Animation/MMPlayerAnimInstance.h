// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/MMBaseAnimInstance.h"
#include "GameData/MMEnums.h"
#include "MMPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMPlayerAnimInstance : public UMMBaseAnimInstance
{
	GENERATED_BODY()

public:
	UMMPlayerAnimInstance();
	
protected:
	// Initialize
	virtual void NativeInitializeAnimation() override;

	// Update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	EClassType ClassType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	uint8 bIsGuard : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	uint8 bIsEquip : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	uint8 bIsHold : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	float Direction;
};

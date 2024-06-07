// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMMechaniteRushATKInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMMechaniteRushATKInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMMechaniteRushATKInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AnimEnd(bool INData) = 0;
	virtual void SettingMoveSpeed(float INDats) = 0;
	virtual void StartMechaniteAnim(int INChoiceMontage) = 0;
	virtual void EndMechaniteAnim() = 0;
	virtual void ChangeCollision() = 0;

	virtual float GetRusingTime() = 0;
	virtual void SetRusingTime(float INData) = 0;

};

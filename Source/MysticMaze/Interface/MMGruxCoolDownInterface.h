// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMGruxCoolDownInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMGruxCoolDownInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMGruxCoolDownInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetCoolDown() = 0;
	virtual void SetCoolDown(float INData) = 0;
	virtual bool GetCoolOn() = 0;
	virtual void SetCoolOn(bool INData) = 0;

};

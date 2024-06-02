// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMMechainteCoolDownInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMMechainteCoolDownInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMMechainteCoolDownInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ResetCoolDownFloat(float INData) = 0;
	virtual void ResetCoolDownbool(bool INData) = 0;

};

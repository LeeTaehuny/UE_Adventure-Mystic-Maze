// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMGruxNormalATKInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMGruxNormalATKInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMGruxNormalATKInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void LeftHook() = 0;
	virtual void RightHook() = 0;
	virtual bool MontagePlaying() = 0;
};

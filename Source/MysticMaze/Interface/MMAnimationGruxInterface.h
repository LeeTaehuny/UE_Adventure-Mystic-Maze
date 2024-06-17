// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMAnimationGruxInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMAnimationGruxInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMAnimationGruxInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool GetDie() = 0;
	virtual bool GetSpawn() = 0;
	virtual bool GetDieMotion() = 0;
	virtual bool GetAngryMode() = 0;
	virtual bool GetAngryModeChangeComplete() = 0;

};

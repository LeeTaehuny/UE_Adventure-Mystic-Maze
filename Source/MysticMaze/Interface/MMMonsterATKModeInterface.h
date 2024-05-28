// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMMonsterATKModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMMonsterATKModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMMonsterATKModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetATKMode(bool INData) = 0;
	virtual bool GetATKMode() = 0;

};

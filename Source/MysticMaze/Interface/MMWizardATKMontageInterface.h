// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMWizardATKMontageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMWizardATKMontageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMWizardATKMontageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartATKMonatage(int INData) = 0;
	virtual void SetNormalATK(bool INData) = 0;
	virtual bool GetMontagePlaying() = 0;

	virtual void StopMove() = 0;

};

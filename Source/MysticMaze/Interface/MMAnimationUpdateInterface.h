// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/MMEnums.h"
#include "MMAnimationUpdateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMAnimationUpdateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMAnimationUpdateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EClassType GetClassType() = 0;
	virtual bool GetIsGuard() = 0;
	virtual bool GetIsEquip() = 0;
	virtual bool GetIsHold() = 0;
};

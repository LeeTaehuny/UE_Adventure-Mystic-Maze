// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMGruxAIDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMMGruxAIDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSTICMAZE_API IMMGruxAIDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 블랙보드에 값을 전달하기 위한 인터페이스
	virtual bool GetDie() = 0;
	virtual bool GetSpawn() = 0;
	virtual bool GetATKMode() = 0;
	virtual bool GetGruxNormalATK() = 0;
	virtual bool GetAngryMode() = 0;
	virtual bool GetAngryModeChangeComplete() = 0;

	virtual float GetHPPercent() = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MMCharacterBase.h"
#include "MMMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMMonsterBase : public AMMCharacterBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bDie : 1;
	
};

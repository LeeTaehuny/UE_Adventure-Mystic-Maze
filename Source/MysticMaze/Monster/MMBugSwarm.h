// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"
#include "MMBugSwarm.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMBugSwarm : public AMMMonsterBase
{
	GENERATED_BODY()

public:
	AMMBugSwarm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float FlyHeight;

protected: // 애니메이션 블루프린트용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 Die : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 Spawn : 1;
	
};

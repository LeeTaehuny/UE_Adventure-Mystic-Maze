// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMMonsterSpawnInterface.h"
#include "Interface/MMAnimationBugSwarmAIInterface.h"

#include "MMGrux.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGrux : public AMMMonsterBase, public IMMMonsterSpawnInterface, public IMMAnimationBugSwarmAIInterface
{
	GENERATED_BODY()
	
public:
	AMMGrux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//IMMAnimationBugSwarmAIInterface : 애니메이션 인터페이스 함수
	FORCEINLINE virtual bool GetDie() override { return bDie; }
	FORCEINLINE virtual bool GetSpawn() override { return bSpawn; }
	FORCEINLINE virtual bool GetDieMotion() override { return bRandomDieMotion; }

	FORCEINLINE virtual void SetSpawnData(bool INspawn) override { bSpawn = INspawn; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bSpawn : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bRandomDieMotion : 1;
	
};

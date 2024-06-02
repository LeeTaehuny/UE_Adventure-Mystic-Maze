// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"
#include "MMGoblinWarrior.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGoblinWarrior : public AMMMonsterBase,
	public IMMMonsterATKChekInterface
{
	GENERATED_BODY()
	

public:
	AMMGoblinWarrior();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	// IMMMonsterATKChekInterface : ������ �����ϴ� �������̽�
	virtual void ATKChecking() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class SpawnType : uint8
{
	BugswarmOnly,
	MechaniteOnly,
	GruxOnly,
	BugswarmAndMechanite,
	BugswarmAndGrux,
	MechaniteAndGrux,
	StrongGrux,

	GoblinWarriorOnly,
	GoblinWizardOnly,
	GoblinWWHalf,
	Warrior23__Wizard13,
	Warrior13__Wizard23,
};

/**
 * 
 */
class MYSTICMAZE_API MMMonsterSpawnType
{
public:
	MMMonsterSpawnType();
	~MMMonsterSpawnType();
};

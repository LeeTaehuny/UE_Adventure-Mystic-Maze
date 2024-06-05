// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Magic/MMMonsterMagicBase.h"

// Sets default values
AMMMonsterMagicBase::AMMMonsterMagicBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoveCompo = CreateDefaultSubobject<UMovementComponent>(TEXT("Movement Component"));
}


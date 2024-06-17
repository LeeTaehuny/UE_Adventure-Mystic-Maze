// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MMCharacterBase.h"
#include "Player/MMStatComponent.h"

AMMCharacterBase::AMMCharacterBase()
{
	// 스탯 컴포넌트 추가
	Stat = CreateDefaultSubobject<UMMStatComponent>(TEXT("StatComponent"));
}

void AMMCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

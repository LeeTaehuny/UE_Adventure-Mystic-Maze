// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMGruxAnim.h"
#include "Interface/MMAnimationGruxInterface.h"

UMMGruxAnim::UMMGruxAnim()
{
	bDie = false;
	bSpawn = true;
	bRandomDieMotion = false;
	bAngryMode = false;
}

void UMMGruxAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	bRandomDieMotion = FMath::RandRange(0, 1);
}

void UMMGruxAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IMMAnimationGruxInterface* monster = Cast<IMMAnimationGruxInterface>(TryGetPawnOwner());
	if (monster)
	{
		bDie = monster->GetDie();
		bSpawn = monster->GetSpawn();
		bRandomDieMotion = monster->GetDieMotion();
		bAngryMode = monster->GetAngryMode();
		bAngryModeChangeComplete = monster->GetAngryModeChangeComplete();
	}
}

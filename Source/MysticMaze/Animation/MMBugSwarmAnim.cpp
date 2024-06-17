// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMBugSwarmAnim.h"
#include "Interface/MMAnimationBugSwarmAIInterface.h"

UMMBugSwarmAnim::UMMBugSwarmAnim()
{
	
}

void UMMBugSwarmAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMBugSwarmAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IMMAnimationBugSwarmAIInterface* monster = Cast<IMMAnimationBugSwarmAIInterface>(TryGetPawnOwner());
	if (monster)
	{
		bDie = monster->GetDie();
		bSpawn = monster->GetSpawn();
		bRandomDieMotion = monster->GetDieMotion();
	}
}

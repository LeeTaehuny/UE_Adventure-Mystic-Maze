// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMGoblinWarriorAnim.h"

#include "Interface/MMAnimationGobWarriorInterface.h"

UMMGoblinWarriorAnim::UMMGoblinWarriorAnim()
{
}

void UMMGoblinWarriorAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMGoblinWarriorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IMMAnimationGobWarriorInterface* monster = Cast<IMMAnimationGobWarriorInterface>(TryGetPawnOwner());
	if (monster)
	{
		bDie = monster->GetDie();
		bRandomDieMotion = monster->GetDieMotion();
	}
}

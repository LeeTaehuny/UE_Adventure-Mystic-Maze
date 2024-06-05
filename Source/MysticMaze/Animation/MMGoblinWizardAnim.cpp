// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMGoblinWizardAnim.h"

#include "Interface/MMAnimationGobWizardInterface.h"

UMMGoblinWizardAnim::UMMGoblinWizardAnim()
{
}

void UMMGoblinWizardAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMGoblinWizardAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IMMAnimationGobWizardInterface* monster = Cast<IMMAnimationGobWizardInterface>(TryGetPawnOwner());
	if (monster)
	{
		bDie = monster->GetDie();
		bRandomDieMotion = monster->GetDieMotion();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMPlayerAnimInstance.h"
#include "Interface/MMAnimationUpdateInterface.h"

UMMPlayerAnimInstance::UMMPlayerAnimInstance()
{
}

void UMMPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IMMAnimationUpdateInterface* Player = Cast<IMMAnimationUpdateInterface>(TryGetPawnOwner());
	if (Player)
	{
		bIsGuard = Player->GetIsGuard();
		ClassType = Player->GetClassType();
		bIsEquip = Player->GetIsEquip();
		bIsHold = Player->GetIsHold();
		bIsCharge = Player->GetIsCharge();

		Direction = CalculateDirection(Velocity, TryGetPawnOwner()->GetActorRotation());
	}
}

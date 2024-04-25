// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMBaseAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMMBaseAnimInstance::UMMBaseAnimInstance()
{
	// 변수 초기화
	MovingThreshould = 3.0f;
}

void UMMBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Owner Actor & CharacterMovement 설정
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMMBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 매 프레임 값 갱신
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsMove = GroundSpeed > MovingThreshould;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MMPlayerControlData.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMPlayerControlData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Pawn
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationPitch : 1;

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationRoll : 1;

	// CharacterMovement
	UPROPERTY(EditAnywhere, Category = Movement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, Category = Movement)
	uint8 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = Movement)
	uint8 bUseControllerDesiredRotation : 1;

	// SpringArm
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bDoCollisionTest : 1;
};

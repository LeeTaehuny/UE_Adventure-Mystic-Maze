// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MMWeapon.h"
#include "MMStaffWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMStaffWeapon : public AMMWeapon
{
	GENERATED_BODY()
	
public:
	AMMStaffWeapon();
	
protected:
	virtual void BeginPlay() override;
};

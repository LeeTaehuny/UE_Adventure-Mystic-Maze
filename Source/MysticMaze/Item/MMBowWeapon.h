// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MMWeapon.h"
#include "MMBowWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMBowWeapon : public AMMWeapon
{
	GENERATED_BODY()
	
public:
	AMMBowWeapon();

public:
	virtual void EquipWeapon(ACharacter* Player) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Quiver", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> QuiverClass;

	UPROPERTY()
	TObjectPtr<AActor> Quiver;

	FName QuiverSocketName;
};

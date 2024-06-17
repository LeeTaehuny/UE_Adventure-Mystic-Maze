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
	virtual void Tick(float DeltaSeconds) override;

public:
	void SpawnEnergyBall();
	void ShootEnergyBall();

protected:
	void SetFireLocation();

	UPROPERTY(VisibleAnywhere, Category = "Quiver", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> EnergyBallClass;

	UPROPERTY()
	TObjectPtr<class AMMEnergyBall> TempEnergyBall;

	FName EnergyBallSocket;

private:
	FVector FireLocation;

};

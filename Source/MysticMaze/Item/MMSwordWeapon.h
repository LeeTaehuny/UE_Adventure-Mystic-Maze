// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MMWeapon.h"
#include "MMSwordWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMSwordWeapon : public AMMWeapon
{
	GENERATED_BODY()
	
public:
	AMMSwordWeapon();

	void EnableCollision();
	void DisableCollision();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> WeaponCollision;
};

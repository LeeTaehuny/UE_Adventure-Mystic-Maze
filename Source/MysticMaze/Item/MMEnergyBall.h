// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMEnergyBall.generated.h"

UCLASS()
class MYSTICMAZE_API AMMEnergyBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AMMEnergyBall();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	void Fire(FVector TargetLocation);

protected:
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> EnergyBallParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> HitParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> EnergyBallCollision;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;

private:
	float Speed = 3000.0f;
	uint8 bIsHit : 1;
};

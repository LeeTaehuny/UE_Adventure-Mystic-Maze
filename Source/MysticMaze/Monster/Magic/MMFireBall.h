// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Magic/MMMonsterMagicBase.h"
#include "MMFireBall.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMFireBall : public AMMMonsterMagicBase
{
	GENERATED_BODY()

public:
	AMMFireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void FireBallBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetDirectionLocation(FVector INData);

private:
	FVector Direction;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> HitParticle;

	float Speed = 3000.0f;

};

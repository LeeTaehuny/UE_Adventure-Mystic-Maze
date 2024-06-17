// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMBladeWave.generated.h"

UCLASS()
class MYSTICMAZE_API AMMBladeWave : public AActor
{
	GENERATED_BODY()
	
public:	
	AMMBladeWave();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	FORCEINLINE void SetDamage(float InDamage) { Damage = InDamage; }
	FORCEINLINE void SetCriticalRate(float InCriticalRate) { CriticalRate = InCriticalRate; }
	FORCEINLINE void SetOwner(AActor* InOwner) { Owner = InOwner; }

protected:
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;

private:
	float Damage;
	float CriticalRate;
};

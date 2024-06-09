// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMStome.generated.h"

UCLASS()
class MYSTICMAZE_API AMMStome : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMStome();

public:
	FORCEINLINE void SetDamage(float InDamage) { Damage = InDamage; }
	FORCEINLINE void SetCriticalRate(float InCriticalRate) { CriticalRate = InCriticalRate; }
	FORCEINLINE void SetOwner(AActor* InOwner) { Owner = InOwner; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void AttackCheck();

	UPROPERTY(VisibleAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> ParticleComponent;
	
	AActor* Owner;
	float CriticalRate;
	float Damage;
	float IntervalTime;
	float SumTime;
	float DestroyTime;
};

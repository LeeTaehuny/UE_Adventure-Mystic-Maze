// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMArrow.generated.h"

UCLASS()
class MYSTICMAZE_API AMMArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AMMArrow();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	void Fire(FVector TargetLocation);

protected:
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FeatherComponent;

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ShaftComponent;

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> IronComponent;

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> ArrowCollision;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;

private:
	float Speed = 2000.0f;
};

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


};

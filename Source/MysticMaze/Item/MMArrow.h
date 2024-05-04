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
	virtual void Tick(float DeltaSeconds) override;

public:
	void Fire();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FeatherComponent;

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ShaftComponent;

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> IronComponent;

	UPROPERTY(VisibleAnywhere, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> ArrowCollision;
};

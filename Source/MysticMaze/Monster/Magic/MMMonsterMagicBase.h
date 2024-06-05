// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/MovementComponent.h"
#include "AIController.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MMMonsterMagicBase.generated.h"

UCLASS()
class MYSTICMAZE_API AMMMonsterMagicBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMMonsterMagicBase();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> Particle;

	TObjectPtr<UShapeComponent> HitComponent;
	TObjectPtr<APawn> Target;

	TObjectPtr<AAIController> MagicController;

	TObjectPtr<UMovementComponent> MoveCompo;
};

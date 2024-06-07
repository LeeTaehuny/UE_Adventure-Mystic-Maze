// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMNPCBase.generated.h"

UCLASS()
class MYSTICMAZE_API AMMNPCBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMMNPCBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Trigger;
};

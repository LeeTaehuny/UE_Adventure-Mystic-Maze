// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMCharacterBase.generated.h"

UCLASS()
class MYSTICMAZE_API AMMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMMCharacterBase();

protected:
	virtual void PostInitializeComponents() override;

protected:
	// Stat
	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMMStatComponent> Stat;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"
#include "MMMechanite.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMMechanite : public AMMMonsterBase
{
	GENERATED_BODY()
	
public:
	AMMMechanite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ATKCombo;

};

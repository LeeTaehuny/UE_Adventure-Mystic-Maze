// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMGoblinCommander.generated.h"

UCLASS()
class MYSTICMAZE_API AMMGoblinCommander : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMGoblinCommander();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GoblinBattleAlgorithm(float DeltaTime);

	void DataReset();

	void SerchGoblin(FVector INData);
	void SerchGoblin(AActor* INData);

protected:

	TObjectPtr<APawn> TargetPlayer;

	UPROPERTY(EditAnywhere, Category = "Goblins Data")
	TArray<TObjectPtr<class AMMGoblinWarrior>> WarriorDatas;

	UPROPERTY(EditAnywhere, Category = "Goblins Data")
	TArray<TObjectPtr<class AMMGoblinWizard>> WizardDatas;

	float TickTime = 0.0f;

};

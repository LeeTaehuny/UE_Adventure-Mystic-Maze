// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster/MMMonsterBase.h"
#include "MMMonsterArea.generated.h"

UCLASS()
class MYSTICMAZE_API AMMMonsterArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMMonsterArea();

	FORCEINLINE void SetCheckLocation(FVector INData) { CheckLocation = INData; }
	void AddMonsterData(AMMMonsterBase* INData);

	bool IfMonsterNull(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnLocation")
	FVector CheckLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monsters Data")
	TArray<TObjectPtr<AMMMonsterBase>> MonsterDAta;

	TObjectPtr<APawn> Player;

	float player_Serch_timer = 0.0f;

};

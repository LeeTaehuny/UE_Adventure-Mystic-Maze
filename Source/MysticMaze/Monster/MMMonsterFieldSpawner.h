// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster/MMMonsterSpawnType.h"
#include "MMMonsterFieldSpawner.generated.h"

UCLASS()
class MYSTICMAZE_API AMMMonsterFieldSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMMonsterFieldSpawner();

	TArray<TObjectPtr<class AMMMonsterBase>> BeginerMonsterSpawn(int INLevel, FVector INCenterLocation);
	TArray<TObjectPtr<class AMMMonsterBase>> IntermediateMonsterSpawn(int INLevel, FVector INCenterLocation);
	TArray<TObjectPtr<class AMMMonsterBase>> ExpertMonsterSpawn(int INLevel, FVector INCenterLocation);

	TArray<TObjectPtr<class AMMMonsterBase>> BugSwarmSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation);
	TArray<TObjectPtr<class AMMMonsterBase>> MechaniteSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation);
	TArray<TObjectPtr<class AMMMonsterBase>> GruxSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation);
	TArray<TObjectPtr<class AMMMonsterBase>> GoblinSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<class UBoxComponent> NorthCollision;
	TObjectPtr<class UBoxComponent> SouthCollision;
	TObjectPtr<class UBoxComponent> EastCollision;
	TObjectPtr<class UBoxComponent> WesthCollision;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMBugSwarm> BugSwarmData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMMechanite> MechaniteData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMGrux> GruxData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMGoblinCommander> GoblinCommanderData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMGoblinWarrior> GoblinWarriorData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMGoblinWizard> GoblinWizardData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	uint8 beginerMonsterSpawnOn : 1;
	uint8 IntermediateMonsterSpawnOn : 1;
	uint8 ExpertMonsterSpawnOn : 1;
	TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonster;

};

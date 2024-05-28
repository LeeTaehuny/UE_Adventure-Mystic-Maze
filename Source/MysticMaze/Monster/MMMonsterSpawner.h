// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster/MMMonsterSpawnType.h"

#include "MMMonsterSpawner.generated.h"

UCLASS()
class MYSTICMAZE_API AMMMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMMonsterSpawner();

	void MonsterSpawn(SpawnType INType, int INLevel, FVector INCenterLocation, FVector INOriginCenterLocation = FVector());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	SpawnType MType = SpawnType::BugswarmOnly;

	// 스폰할 몬스터의 블루프린트 데이터를 저장하기 위한 변수들
	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMBugSwarm> BugSwarmData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMMechanite> MechaniteData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMGrux> GruxData;

	TArray<TObjectPtr<class AMMMonsterArea>> Areas;

	// TArray<TObjectPtr<class AMMMonsterBase>> Monster;

	// 스폰 중심 포인트를 기록하기 위한 벡터 변수
	// 이곳을 중심으로 몬스터들을 사방에 스폰함
	// 배열의 크기에 따라 소환되는 위치가 다 다름
	// FVector SpawnPoint;
};

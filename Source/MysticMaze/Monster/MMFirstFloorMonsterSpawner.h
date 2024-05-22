// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// 몬스터 인크루드
#include "Monster/MMBugSwarm.h"
#include "Monster/MMMechanite.h"

#include "MMFirstFloorMonsterSpawner.generated.h"

UCLASS()
class MYSTICMAZE_API AMMFirstFloorMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMFirstFloorMonsterSpawner();

	void SetSpawnPoint(FVector INPoint) { SpawnPoint.Add(INPoint); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MonsterSpawn(int INInteger);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// 스폰할 몬스터의 블루프린트 데이터를 저장하기 위한 변수들
	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<AMMBugSwarm> BugSwarmData;
	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<AMMMechanite> MechaniteData;

	TObjectPtr<APawn> Monster;

	// 몇 번 타입의 몬스터 케이스를 스폰할지를 정하는 변수
	uint8 SpawnType : 3;

	// 스폰 중심 포인트를 기록하기 위한 벡터 변수
	// 이곳을 중심으로 몬스터들을 사방에 스폰함
	// 배열의 크기에 따라 소환되는 위치가 다 다름
	TArray<FVector> SpawnPoint;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMFirstFloorMonsterSpawner.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMMFirstFloorMonsterSpawner::AMMFirstFloorMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("FirstFloorSpawner");
}

// Called when the game starts or when spawned
void AMMFirstFloorMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	// 스폰할 룸의 타입
	// 타입에 따라 스폰되는 몬스터가 달라짐
	SpawnType = FMath::RandRange(0, 8);

	for (int i = 0; i < SpawnPoint.Num(); i++)
	{
		// 입력받은 스폰포인트의 수에 따라 몬스터가 배로 늘어남
		MonsterSpawn(i);
	}	
}

void AMMFirstFloorMonsterSpawner::MonsterSpawn(int INInteger)
{
	AMMBugSwarm* MonsterBugSwarm;
	AMMMechanite* MonsterMechanite;
	float RandomXLocation;
	float RandomYLocation;
	SpawnType = 1;
	switch (SpawnType)
	{
	case 0:
		for (int i = 0; i < 8; i++)
		{
			MonsterBugSwarm = GetWorld()->SpawnActorDeferred<AMMBugSwarm>(BugSwarmData, FTransform(FVector()));
			if (MonsterBugSwarm)
			{
				MonsterBugSwarm->SetCenterLocation(SpawnPoint[INInteger]);

				// 초기화 완료
				UGameplayStatics::FinishSpawningActor(MonsterBugSwarm, FTransform(FVector()));
			}

			RandomXLocation = FMath::RandRange(-1800, 1800);
			RandomYLocation = FMath::RandRange(-1800, 1800);
			MonsterBugSwarm->SetActorLocation(FVector(
				SpawnPoint[INInteger].X + RandomXLocation,
				SpawnPoint[INInteger].Y + RandomYLocation,
				1100));
		}
		break;

	case 1:
		for (int i = 0; i < 2; i++)
		{
			MonsterMechanite = GetWorld()->SpawnActorDeferred<AMMMechanite>(MechaniteData, FTransform(FVector()));
			if (MonsterMechanite)
			{
				//MonsterMechanite->SetCenterLocation(SpawnPoint[INInteger]);

				// 초기화 완료
				UGameplayStatics::FinishSpawningActor(MonsterMechanite, FTransform(FVector()));
			}

			RandomXLocation = FMath::RandRange(-1800, 1800);
			RandomYLocation = FMath::RandRange(-1800, 1800);
			MonsterMechanite->SetActorLocation(FVector(
				SpawnPoint[INInteger].X + RandomXLocation,
				SpawnPoint[INInteger].Y + RandomYLocation,
				1200));
		}
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		break;

	case 6:
		break;

	case 7:
		break;

	default:
		break;
	}
}

// Called every frame
void AMMFirstFloorMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


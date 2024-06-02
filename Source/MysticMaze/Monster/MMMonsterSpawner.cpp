// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterSpawner.h"

// 몬스터 인크루드
#include "Monster/MMBugSwarm.h"
#include "Monster/MMMechanite.h"
#include "Monster/MMGrux.h"

#include "Monster/MMMonsterArea.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AMMMonsterSpawner::AMMMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterSpawner::MonsterSpawn(SpawnType INType, int INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsters;

	AMMBugSwarm* MonsterBugSwarm;
	AMMMechanite* MonsterMechanite;
	AMMGrux* MonsterGrux;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;
	UWorld* world = GetWorld();

	INType = SpawnType::GruxOnly;
	
	switch (INType)
	{
	case SpawnType::BugswarmOnly:
		for (int i = 0; i < 8; i++)
		{
			MonsterBugSwarm = world->SpawnActorDeferred<AMMBugSwarm>(BugSwarmData, FTransform(FVector()));
			if (MonsterBugSwarm)
			{
				// 초기화 완료
				UGameplayStatics::FinishSpawningActor(MonsterBugSwarm, FTransform(FVector()));
			}

			// 몬스터의 스폰을 기준 점에서부터 랜던한 곳으로 날려버리기 위한 코드
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterBugSwarm->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1100));

			MonsterBugSwarm->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGrux);
		}
		break;

	case SpawnType::MechaniteOnly:
		for (int i = 0; i < 3; i++)
		{
			MonsterMechanite = world->SpawnActorDeferred<AMMMechanite>(MechaniteData, FTransform(FVector()));
			if (MonsterMechanite)
			{
				

				// 초기화 완료
				UGameplayStatics::FinishSpawningActor(MonsterMechanite, FTransform(FVector()));
			}

			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterMechanite->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1200));

			MonsterMechanite->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGrux);
		}
		break;

	case SpawnType::GruxOnly:
		for (int i = 0; i < 1; i++)
		{
			MonsterGrux = world->SpawnActorDeferred<AMMGrux>(GruxData, FTransform(FVector()));
			if (MonsterGrux)
			{


				// 초기화 완료
				UGameplayStatics::FinishSpawningActor(MonsterGrux, FTransform(FVector()));
			}

			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterGrux->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1200));

			MonsterGrux->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGrux);
		}
		break;

	case SpawnType::BugswarmAndMechanite:
		break;

	case SpawnType::BugswarmAndGrux:
		break;

	case SpawnType::MechaniteAndGrux:
		break;

	case SpawnType::StrongGrux:
		break;

	default:
		break;
	}

	return SpawnMonsters;
}

// Called when the game starts or when spawned
void AMMMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}
//
//// Called every frame
void AMMMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


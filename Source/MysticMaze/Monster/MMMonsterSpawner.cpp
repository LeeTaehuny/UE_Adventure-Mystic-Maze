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

void AMMMonsterSpawner::MonsterSpawn(SpawnType INType, int INLevel, FVector INCenterLocation, FVector INOriginCenterLocation)
{
	AMMBugSwarm* MonsterBugSwarm;
	AMMMechanite* MonsterMechanite;
	//AMMGrux* MonsterGrux;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;
	UWorld* world = GetWorld();

	Areas.Add(world->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass()));

	INType = SpawnType::BugswarmOnly;
	
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

			if (INOriginCenterLocation != FVector())
			{
				// 만약 오리진 매개변수의 값이 제로라면 혹은 아무런 값도 들어가있지 않다고 한다면
				// 기존에 입력받은 스폰 로케이션을 센터 좌표로 할당
				MonsterBugSwarm->SetCenterLocation(INCenterLocation);
				Areas[Areas.Num() - 1]->SetCheckLocation(INOriginCenterLocation);
			}
			else
			{
				// 그렇지 않다면 본래 오리진 좌표를 센터 좌표에 할당
				MonsterBugSwarm->SetCenterLocation(INCenterLocation);
				Areas[Areas.Num() - 1]->SetCheckLocation(INCenterLocation);
			}
			Areas[Areas.Num() - 1]->AddMonsterData(MonsterBugSwarm);
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

			if (INOriginCenterLocation != FVector())
			{
				// 만약 오리진 매개변수의 값이 제로라면 혹은 아무런 값도 들어가있지 않다고 한다면
				// 기존에 입력받은 스폰 로케이션을 센터 좌표로 할당
				MonsterMechanite->SetCenterLocation(INCenterLocation);
				Areas[Areas.Num() - 1]->SetCheckLocation(INOriginCenterLocation);
			}
			else
			{
				// 그렇지 않다면 본래 오리진 좌표를 센터 좌표에 할당
				MonsterMechanite->SetCenterLocation(INCenterLocation);
				Areas[Areas.Num() - 1]->SetCheckLocation(INCenterLocation);
			}
			Areas[Areas.Num() - 1]->AddMonsterData(MonsterMechanite);
		}
		break;

	case SpawnType::GruxOnly:
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
}

// Called when the game starts or when spawned
void AMMMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	//MonsterSpawn(SpawnType::BugswarmOnly, 1, FVector(0, 0, 1000));
}
//
//// Called every frame
//void AMMMonsterSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


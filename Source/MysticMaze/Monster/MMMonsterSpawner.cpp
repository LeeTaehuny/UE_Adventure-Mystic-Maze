// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterSpawner.h"

// ���� ��ũ���
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
				// �ʱ�ȭ �Ϸ�
				UGameplayStatics::FinishSpawningActor(MonsterBugSwarm, FTransform(FVector()));
			}

			// ������ ������ ���� ���������� ������ ������ ���������� ���� �ڵ�
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterBugSwarm->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1100));

			if (INOriginCenterLocation != FVector())
			{
				// ���� ������ �Ű������� ���� ���ζ�� Ȥ�� �ƹ��� ���� ������ �ʴٰ� �Ѵٸ�
				// ������ �Է¹��� ���� �����̼��� ���� ��ǥ�� �Ҵ�
				MonsterBugSwarm->SetCenterLocation(INCenterLocation);
				Areas[Areas.Num() - 1]->SetCheckLocation(INOriginCenterLocation);
			}
			else
			{
				// �׷��� �ʴٸ� ���� ������ ��ǥ�� ���� ��ǥ�� �Ҵ�
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
				

				// �ʱ�ȭ �Ϸ�
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
				// ���� ������ �Ű������� ���� ���ζ�� Ȥ�� �ƹ��� ���� ������ �ʴٰ� �Ѵٸ�
				// ������ �Է¹��� ���� �����̼��� ���� ��ǥ�� �Ҵ�
				MonsterMechanite->SetCenterLocation(INCenterLocation);
				Areas[Areas.Num() - 1]->SetCheckLocation(INOriginCenterLocation);
			}
			else
			{
				// �׷��� �ʴٸ� ���� ������ ��ǥ�� ���� ��ǥ�� �Ҵ�
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


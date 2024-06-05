// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterSpawner.h"

// ���� ��ũ���
#include "Monster/MMBugSwarm.h"
#include "Monster/MMMechanite.h"
#include "Monster/MMGrux.h"
#include "Monster/MMGoblinCommander.h"
#include "Monster/MMGoblinWarrior.h"
#include "Monster/MMGoblinWizard.h"

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
	AMMGoblinCommander* MonsterGoblinCommander;
	AMMGoblinWarrior* MonsterGobinWarrior;
	AMMGoblinWizard* MonsterGobinWizard;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;
	UWorld* world = GetWorld();

	INType = SpawnType::GoblinWWHalf;
	
	switch (INType)
	{
		// �� �����δ� 1���� //
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
				

				// �ʱ�ȭ �Ϸ�
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


				// �ʱ�ȭ �Ϸ�
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
		// �� ���δ� 1���� //

		// �� �����δ� 2���� //
	case SpawnType::GoblinWarriorOnly:

		MonsterGoblinCommander = world->SpawnActor<AMMGoblinCommander>(GoblinCommanderData);

		for (int i = 0; i < 8; i++)
		{
			MonsterGobinWarrior = world->SpawnActorDeferred<AMMGoblinWarrior>(GoblinWarriorData, FTransform(FVector()));
			if (MonsterGobinWarrior)
			{
				// �ʱ�ȭ �Ϸ�
				UGameplayStatics::FinishSpawningActor(MonsterGobinWarrior, FTransform(FVector()));
			}

			// ������ ������ ���� ���������� ������ ������ ���������� ���� �ڵ�
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterGobinWarrior->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1100));

			MonsterGobinWarrior->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWarrior);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWarrior);
		}
		break;

	case SpawnType::GoblinWizardOnly:

		MonsterGoblinCommander = world->SpawnActor<AMMGoblinCommander>(GoblinCommanderData);

		for (int i = 0; i < 8; i++)
		{
			MonsterGobinWizard = world->SpawnActorDeferred<AMMGoblinWizard>(GoblinWizardData, FTransform(FVector()));
			if (MonsterGobinWizard)
			{
				// �ʱ�ȭ �Ϸ�
				UGameplayStatics::FinishSpawningActor(MonsterGobinWizard, FTransform(FVector()));
			}

			// ������ ������ ���� ���������� ������ ������ ���������� ���� �ڵ�
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterGobinWizard->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1100));

			MonsterGobinWizard->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWizard);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWizard);
		}
		break;

	case SpawnType::GoblinWWHalf:

		MonsterGoblinCommander = world->SpawnActor<AMMGoblinCommander>(GoblinCommanderData);

		for (int i = 0; i < 4; i++)
		{
			MonsterGobinWizard = world->SpawnActorDeferred<AMMGoblinWizard>(GoblinWizardData, FTransform(FVector()));
			if (MonsterGobinWizard)
			{
				// �ʱ�ȭ �Ϸ�
				UGameplayStatics::FinishSpawningActor(MonsterGobinWizard, FTransform(FVector()));
			}

			// ������ ������ ���� ���������� ������ ������ ���������� ���� �ڵ�
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterGobinWizard->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1200));

			MonsterGobinWizard->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWizard);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWizard);
		}
		for (int i = 0; i < 4; i++)
		{
			MonsterGobinWarrior = world->SpawnActorDeferred<AMMGoblinWarrior>(GoblinWarriorData, FTransform(FVector()));
			if (MonsterGobinWarrior)
			{
				// �ʱ�ȭ �Ϸ�
				UGameplayStatics::FinishSpawningActor(MonsterGobinWarrior, FTransform(FVector()));
			}

			// ������ ������ ���� ���������� ������ ������ ���������� ���� �ڵ�
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterGobinWarrior->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1200));

			UE_LOG(LogTemp, Display, TEXT("X : %f, Y : %f, Z : %f"), MonsterGobinWarrior->GetActorLocation().X, MonsterGobinWarrior->GetActorLocation().Y, MonsterGobinWarrior->GetActorLocation().Z);

			MonsterGobinWarrior->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWarrior);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWarrior);
		}
		
		break;

	case SpawnType::Warrior23__Wizard13:
		break;

	case SpawnType::Warrior13__Wizard23:
		break;
		// �� ���δ� 2���� //

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


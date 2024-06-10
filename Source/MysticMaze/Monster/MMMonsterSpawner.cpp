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
	PrimaryActorTick.bCanEverTick = false;

	
}

TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterSpawner::MonsterSpawn(SpawnType INType, int INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsters;

	AMMBugSwarm* MonsterBugSwarm = NULL;
	AMMMechanite* MonsterMechanite = NULL;
	AMMGrux* MonsterGrux = NULL;
	AMMGoblinCommander* MonsterGoblinCommander = NULL;
	AMMGoblinWarrior* MonsterGobinWarrior = NULL;
	AMMGoblinWizard* MonsterGobinWizard = NULL;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;
	UWorld* world = GetWorld();

	//INType = SpawnType::MechaniteOnly;
	
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
		
			
			MonsterBugSwarm->SetStatLevel(INLevel);
			MonsterBugSwarm->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterBugSwarm);
		}
		break;
		 
	case SpawnType::MechaniteOnly:
		for (int i = 0; i < 5; i++)
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

			MonsterMechanite->SetStatLevel(INLevel);
			MonsterMechanite->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterMechanite);
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

			MonsterGrux->SetStatLevel(INLevel);
			MonsterGrux->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGrux);
		}
		break;

	case SpawnType::BugswarmAndMechanite:

		for (int i = 0; i < 4; i++)
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
		
			MonsterBugSwarm->SetStatLevel(INLevel);
			MonsterBugSwarm->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterBugSwarm);
		}

		for (int i = 0; i < 1; i++)
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

			MonsterMechanite->SetStatLevel(INLevel);
			MonsterMechanite->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterMechanite);
		}
		break;

	case SpawnType::BugswarmAndGrux:

		for (int i = 0; i < 2; i++)
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
		
			MonsterBugSwarm->SetStatLevel(INLevel);
			MonsterBugSwarm->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterBugSwarm);
		}

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

	case SpawnType::MechaniteAndGrux:

		for (int i = 0; i < 2; i++)
		{
			MonsterMechanite = world->SpawnActorDeferred<AMMMechanite>(MechaniteData, FTransform(FVector()));
			if (MonsterMechanite)
			{
				// �ʱ�ȭ �Ϸ�
				UGameplayStatics::FinishSpawningActor(MonsterMechanite, FTransform(FVector()));
			}

			// ������ ������ ���� ���������� ������ ������ ���������� ���� �ڵ�
			RandomXLocation = FMath::RandRange(-Distance, Distance);
			RandomYLocation = FMath::RandRange(-Distance, Distance);
			MonsterMechanite->SetActorLocation(FVector(
				INCenterLocation.X + RandomXLocation,
				INCenterLocation.Y + RandomYLocation,
				1100));

			MonsterMechanite->SetStatLevel(INLevel);
			MonsterMechanite->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterMechanite);
		}

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

			MonsterGrux->SetStatLevel(INLevel);
			MonsterGrux->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGrux);
		}
		break;

	case SpawnType::StrongGrux:
		for (int i = 0; i < 2; i++)
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

			MonsterGrux->SetStatLevel(INLevel);
			MonsterGrux->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGrux);
		}
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

			MonsterGobinWarrior->SetStatLevel(INLevel);
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

			MonsterGobinWizard->SetStatLevel(INLevel);
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

			MonsterGobinWizard->SetStatLevel(INLevel);
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

			MonsterGobinWarrior->SetStatLevel(INLevel);
			MonsterGobinWarrior->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWarrior);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWarrior);
		}
		
		break;

	case SpawnType::Warrior23__Wizard13:

		MonsterGoblinCommander = world->SpawnActor<AMMGoblinCommander>(GoblinCommanderData);

		for (int i = 0; i < 6; i++)
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

			MonsterGobinWizard->SetStatLevel(INLevel);
			MonsterGobinWizard->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWizard);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWizard);
		}
		for (int i = 0; i < 2; i++)
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

			MonsterGobinWarrior->SetStatLevel(INLevel);
			MonsterGobinWarrior->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWarrior);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWarrior);
		}
		break;

	case SpawnType::Warrior13__Wizard23:

		MonsterGoblinCommander = world->SpawnActor<AMMGoblinCommander>(GoblinCommanderData);

		for (int i = 0; i < 2; i++)
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

			MonsterGobinWizard->SetStatLevel(INLevel);
			MonsterGobinWizard->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWizard);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWizard);
		}
		for (int i = 0; i < 6; i++)
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

			MonsterGobinWarrior->SetStatLevel(INLevel);
			MonsterGobinWarrior->SetCenterLocation(INCenterLocation);
			SpawnMonsters.Add(MonsterGobinWarrior);
			MonsterGoblinCommander->SerchGoblin(MonsterGobinWarrior);
		}
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


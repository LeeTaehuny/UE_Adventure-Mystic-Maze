// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterFieldSpawner.h"
#include "Components/BoxComponent.h"

#include "Monster/MMBugSwarm.h"
#include "Monster/MMMechanite.h"
#include "Monster/MMGrux.h"
#include "Monster/MMGoblinCommander.h"
#include "Monster/MMGoblinWarrior.h"
#include "Monster/MMGoblinWizard.h"

#include "Interface/MMStatusInterface.h"

#include "Monster/MMMonsterArea.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMMMonsterFieldSpawner::AMMMonsterFieldSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	beginerMonsterSpawnOn = false;
	IntermediateMonsterSpawnOn = false;
	ExpertMonsterSpawnOn = false;

	NorthCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision"));
	NorthCollision->SetupAttachment(RootComponent);
	NorthCollision->SetCollisionProfileName(TEXT("NoCollision"));

	SouthCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision"));
	SouthCollision->SetupAttachment(RootComponent);
	SouthCollision->SetCollisionProfileName(TEXT("NoCollision"));

	EastCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision"));
	EastCollision->SetupAttachment(RootComponent);
	EastCollision->SetCollisionProfileName(TEXT("NoCollision"));

	WesthCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision"));
	WesthCollision->SetupAttachment(RootComponent);
	WesthCollision->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FClassFinder<AMMBugSwarm> BugSwarmClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/BugSwarm/BP_BugSwarm.BP_BugSwarm_C'"));
	if (BugSwarmClassRef.Succeeded())
	{
		BugSwarmData = BugSwarmClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMMechanite> MechaniteClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Mechanite/BP_Mechanite.BP_Mechanite_C'"));
	if (MechaniteClassRef.Succeeded())
	{
		MechaniteData = MechaniteClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMGrux> GruxClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Grux/BP_Grux.BP_Grux_C'"));
	if (GruxClassRef.Succeeded())
	{
		GruxData = GruxClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinCommander> CommandClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/BP_GoblinCommander.BP_GoblinCommander_C'"));
	if (CommandClassRef.Succeeded())
	{
		GoblinCommanderData = CommandClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinWarrior> WarriorClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/Warrior/BP_GoblinWarrior.BP_GoblinWarrior_C'"));
	if (WarriorClassRef.Succeeded())
	{
		GoblinWarriorData = WarriorClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinWizard> WizardClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/Wizard/BP_GoblinWizard.BP_GoblinWizard_C'"));
	if (WizardClassRef.Succeeded())
	{
		GoblinWizardData = WizardClassRef.Class;
	}
}

// Called when the game starts or when spawned
void AMMMonsterFieldSpawner::BeginPlay()
{
	Super::BeginPlay();

	NorthCollision->SetCollisionProfileName(TEXT("MMLongATK"));
	NorthCollision->SetWorldLocation(FVector(870.0f, 8470.0f, 90.0f));
	NorthCollision->SetWorldScale3D(FVector(6, 1, 1));

	SouthCollision->SetCollisionProfileName(TEXT("MMLongATK"));
	SouthCollision->SetWorldLocation(FVector(2610.0f, -4570.0f, 90.0f));;
	SouthCollision->SetWorldScale3D(FVector(6, 1, 1));

	EastCollision->SetCollisionProfileName(TEXT("MMLongATK"));
	EastCollision->SetWorldLocation(FVector(-4620.0, 2930.0, 90.0f));;
	EastCollision->SetWorldScale3D(FVector(1, 6, 1));

	WesthCollision->SetCollisionProfileName(TEXT("MMLongATK"));
	WesthCollision->SetWorldLocation(FVector(8630.0f, 1220.0f, 90.0f));;
	WesthCollision->SetWorldScale3D(FVector(1, 6, 1));

	NorthCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::NorthBeginOverlap);
	NorthCollision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::NorthEndOverlap);
	EastCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::EastBeginOverlap);
	EastCollision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::EastEndOverlap);
	WesthCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::WastBeginOverlap);
	WesthCollision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::WastEndOverlap);
	SouthCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::SouthBeginOverlap);
	SouthCollision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterFieldSpawner::SouthEndOverlap);
}

// Called every frame
void AMMMonsterFieldSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::BeginerMonsterSpawn(int INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsters;

	SpawnMonsters = MechaniteSpawn(3, INLevel, INCenterLocation);

	return SpawnMonsters;
}
TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::IntermediateMonsterSpawn(int INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsters;

	int RandomSpawnNumber = FMath::RandRange(1, 2);
	switch (RandomSpawnNumber)
	{
	case 1:
		SpawnMonsters = BugSwarmSpawn(7, INLevel, INCenterLocation);
		break;

	case 2:
		SpawnMonsters = MechaniteSpawn(5, INLevel, INCenterLocation);
		break;

	default:
		break;
	}

	return SpawnMonsters;
}
TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::ExpertMonsterSpawn(int INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsters;

	int RandomSpawnNumber = FMath::RandRange(1, 2);
	switch (RandomSpawnNumber)
	{
	case 1:
		SpawnMonsters = GruxSpawn(1, INLevel, INCenterLocation);
		break;

	case 2:
		SpawnMonsters = GoblinSpawn(10, INLevel, INCenterLocation);
		break;

	default:
		break;
	}

	return SpawnMonsters;
}



TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::BugSwarmSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> BugSwarms;

	AMMBugSwarm* MonsterBugSwarm = NULL;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;

	for (int i = 0; i < INNumber; i++)
	{
		MonsterBugSwarm = GetWorld()->SpawnActorDeferred<AMMBugSwarm>(BugSwarmData, FTransform(FVector()));
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
			INCenterLocation.Z));


		MonsterBugSwarm->SetStatLevel(INLevel);
		MonsterBugSwarm->SetCenterLocation(INCenterLocation);
		BugSwarms.Add(MonsterBugSwarm);
	}
	return BugSwarms;
}
TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::MechaniteSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> Mechanites;

	AMMMechanite* MonsterMechanite = NULL;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;

	for (int i = 0; i < INNumber; i++)
	{
		MonsterMechanite = GetWorld()->SpawnActorDeferred<AMMMechanite>(MechaniteData, FTransform(FVector()));
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
			INCenterLocation.Z));

		MonsterMechanite->SetStatLevel(INLevel);
		MonsterMechanite->SetCenterLocation(INCenterLocation);
		Mechanites.Add(MonsterMechanite);
	}

	return Mechanites;
}
TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::GruxSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> Gruxs;

	AMMGrux* MonsterGrux = NULL;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;

	for (int i = 0; i < INNumber; i++)
	{
		MonsterGrux = GetWorld()->SpawnActorDeferred<AMMGrux>(GruxData, FTransform(FVector()));
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
			INCenterLocation.Z));

		MonsterGrux->SetStatLevel(INLevel);
		MonsterGrux->SetCenterLocation(INCenterLocation);
		Gruxs.Add(MonsterGrux);
	}

	return Gruxs;
}
TArray<TObjectPtr<class AMMMonsterBase>> AMMMonsterFieldSpawner::GoblinSpawn(int32 INNumber, int32 INLevel, FVector INCenterLocation)
{
	TArray<TObjectPtr<class AMMMonsterBase>> Goblins;

	AMMGoblinCommander* MonsterGoblinCommander = NULL;
	AMMGoblinWarrior* MonsterGobinWarrior = NULL;
	AMMGoblinWizard* MonsterGobinWizard = NULL;
	float RandomXLocation;
	float RandomYLocation;
	float Distance = 1400;

	MonsterGoblinCommander = GetWorld()->SpawnActor<AMMGoblinCommander>(GoblinCommanderData);

	int32 Random = FMath::RandRange(0, INNumber);

	int32 Warrior = INNumber - Random;
	int32 Wizard = Random;

	for (int i = 0; i < Wizard; i++)
	{
		MonsterGobinWizard = GetWorld()->SpawnActorDeferred<AMMGoblinWizard>(GoblinWizardData, FTransform(FVector()));
		if (MonsterGobinWizard)
		{
			// 초기화 완료
			UGameplayStatics::FinishSpawningActor(MonsterGobinWizard, FTransform(FVector()));
		}

		// 몬스터의 스폰을 기준 점에서부터 랜던한 곳으로 날려버리기 위한 코드
		RandomXLocation = FMath::RandRange(-Distance, Distance);
		RandomYLocation = FMath::RandRange(-Distance, Distance);
		MonsterGobinWizard->SetActorLocation(FVector(
			INCenterLocation.X + RandomXLocation,
			INCenterLocation.Y + RandomYLocation,
			INCenterLocation.Z));

		MonsterGobinWizard->SetStatLevel(INLevel);
		MonsterGobinWizard->SetCenterLocation(INCenterLocation);
		Goblins.Add(MonsterGobinWizard);
		MonsterGoblinCommander->SerchGoblin(MonsterGobinWizard);
	}
	for (int i = 0; i < Warrior; i++)
	{
		MonsterGobinWarrior = GetWorld()->SpawnActorDeferred<AMMGoblinWarrior>(GoblinWarriorData, FTransform(FVector()));
		if (MonsterGobinWarrior)
		{
			// 초기화 완료
			UGameplayStatics::FinishSpawningActor(MonsterGobinWarrior, FTransform(FVector()));
		}

		// 몬스터의 스폰을 기준 점에서부터 랜던한 곳으로 날려버리기 위한 코드
		RandomXLocation = FMath::RandRange(-Distance, Distance);
		RandomYLocation = FMath::RandRange(-Distance, Distance);
		MonsterGobinWarrior->SetActorLocation(FVector(
			INCenterLocation.X + RandomXLocation,
			INCenterLocation.Y + RandomYLocation,
			INCenterLocation.Z));

		UE_LOG(LogTemp, Display, TEXT("X : %f, Y : %f, Z : %f"), MonsterGobinWarrior->GetActorLocation().X, MonsterGobinWarrior->GetActorLocation().Y, MonsterGobinWarrior->GetActorLocation().Z);

		MonsterGobinWarrior->SetStatLevel(INLevel);
		MonsterGobinWarrior->SetCenterLocation(INCenterLocation);
		Goblins.Add(MonsterGobinWarrior);
		MonsterGoblinCommander->SerchGoblin(MonsterGobinWarrior);
	}

	return Goblins;
}





void AMMMonsterFieldSpawner::NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("North Hit In"));
}
void AMMMonsterFieldSpawner::NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AMMMonsterFieldSpawner::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("beginer In"));

	if (beginerMonsterSpawnOn)
	{
		beginerMonsterSpawnOn = false;
		IntermediateMonsterSpawnOn = false;
		ExpertMonsterSpawnOn = false;
		UE_LOG(LogTemp, Display, TEXT("On : %d"), SpawnMonster.Num());
		for (int i = 0; i < SpawnMonster.Num(); i++)
		{
			SpawnMonster[i]->Destroy();
			SpawnMonster[i] = NULL;
		}

		SpawnMonster.RemoveAll([](AMMMonsterBase* Monster)
			{
				return Monster == nullptr || Monster->IsPendingKill();
			});
	}
	else
	{
		beginerMonsterSpawnOn = true;

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		int32 PlLevel = 0;
		IMMStatusInterface* PlayerData = Cast<IMMStatusInterface>(PlayerPawn);
		if (PlayerData)
		{
			PlLevel = PlayerData->GetStatComponent()->GetCurrentLevel();
		}

		TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsterData;
		SpawnMonsterData = BeginerMonsterSpawn(PlLevel, FVector(13060.0f, 2060.0f, -440.0f));
		AMMMonsterArea* Num1MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num1MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num1MonsterArea->SetCheckLocation(FVector(-13060.0f, 2060.0f, -440.0f));
		}

		SpawnMonsterData = BeginerMonsterSpawn(PlLevel, FVector(11200.0, 10880.0, -440.0));
		AMMMonsterArea* Num2MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num2MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num2MonsterArea->SetCheckLocation(FVector(11200.0, 10880.0, -440.0));
		}

		SpawnMonsterData = BeginerMonsterSpawn(PlLevel, FVector(12430.0, 6440.0, -440.0));
		AMMMonsterArea* Num3MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num3MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num3MonsterArea->SetCheckLocation(FVector(12430.0, 6440.0, -440.0));
		}

		SpawnMonsterData = BeginerMonsterSpawn(PlLevel, FVector(14100.0, -6920.0, -440.0));
		AMMMonsterArea* Num4MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num4MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num4MonsterArea->SetCheckLocation(FVector(14100.0, -6920.0, -440.0));
		}

	}
}
void AMMMonsterFieldSpawner::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AMMMonsterFieldSpawner::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Expert Hit In"));

	if (ExpertMonsterSpawnOn)
	{
		beginerMonsterSpawnOn = false;
		IntermediateMonsterSpawnOn = false;
		ExpertMonsterSpawnOn = false;
		UE_LOG(LogTemp, Display, TEXT("On : %d"), SpawnMonster.Num());
		for (int i = 0; i < SpawnMonster.Num(); i++)
		{
			SpawnMonster[i]->Destroy();
			SpawnMonster[i] = NULL;
		}

		SpawnMonster.RemoveAll([](AMMMonsterBase* Monster)
			{
				return Monster == nullptr || Monster->IsPendingKill();
			});
	}
	else
	{
		ExpertMonsterSpawnOn = true;

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		int32 PlLevel = 0;
		IMMStatusInterface* PlayerData = Cast<IMMStatusInterface>(PlayerPawn);
		if (PlayerData)
		{
			PlLevel = PlayerData->GetStatComponent()->GetCurrentLevel();

			PlLevel += 2;
		}

		TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsterData;
		SpawnMonsterData = ExpertMonsterSpawn(PlLevel, FVector(-10660.0, 9000.0, -440.0));
		AMMMonsterArea* Num1MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);
			
			Num1MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num1MonsterArea->SetCheckLocation(FVector(-10660.0, 9000.0, -440.0));
		}

		SpawnMonsterData = ExpertMonsterSpawn(PlLevel, FVector(-11890.0, 3710.0, -440.0));
		AMMMonsterArea* Num2MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num2MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num2MonsterArea->SetCheckLocation(FVector(-11890.0, 3710.0, -440.0));
		}

		SpawnMonsterData = ExpertMonsterSpawn(PlLevel, FVector(-11770.0, -2240.0, -440.0));
		AMMMonsterArea* Num3MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num3MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num3MonsterArea->SetCheckLocation(FVector(-11770.0, -2240.0, -440.0));
		}

		SpawnMonsterData = ExpertMonsterSpawn(PlLevel, FVector(-10820.0, -6210.0, -440.0));
		AMMMonsterArea* Num4MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);

			Num4MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num4MonsterArea->SetCheckLocation(FVector(-10820.0, -6210.0, -440.0));
		}

	}
}
void AMMMonsterFieldSpawner::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AMMMonsterFieldSpawner::SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Intermediate Hit In"));

	if (IntermediateMonsterSpawnOn)
	{
		beginerMonsterSpawnOn = false;
		IntermediateMonsterSpawnOn = false;
		ExpertMonsterSpawnOn = false;
		UE_LOG(LogTemp, Display, TEXT("On : %d"), SpawnMonster.Num());
		for (int i = 0; i < SpawnMonster.Num(); i++)
		{
			SpawnMonster[i]->Destroy();
			SpawnMonster[i] = NULL;
		}

		SpawnMonster.RemoveAll([](AMMMonsterBase* Monster)
			{
				return Monster == nullptr || Monster->IsPendingKill();
			});
	}
	else
	{
		
		IntermediateMonsterSpawnOn = true;

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		int32 PlLevel = 0;
		IMMStatusInterface* PlayerData = Cast<IMMStatusInterface>(PlayerPawn);
		if (PlayerData)
		{
			PlLevel = PlayerData->GetStatComponent()->GetCurrentLevel();
			PlLevel += 1;
		}

		TArray<TObjectPtr<class AMMMonsterBase>> SpawnMonsterData;
		SpawnMonsterData = IntermediateMonsterSpawn(PlLevel, FVector(-4090.0, -8200.0, -440.0));
		AMMMonsterArea* Num1MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);
			Num1MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num1MonsterArea->SetCheckLocation(FVector(4090.0, -8200.0, -440.0));
		}

		SpawnMonsterData = IntermediateMonsterSpawn(PlLevel, FVector(710.0, -10510.0, -440.0));
		AMMMonsterArea* Num2MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);
			Num2MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num2MonsterArea->SetCheckLocation(FVector(710.0, -10510.0, -440.0));
		}

		SpawnMonsterData = IntermediateMonsterSpawn(PlLevel, FVector(6400.0, -10150.0, -440.0));
		AMMMonsterArea* Num3MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);
			Num3MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num3MonsterArea->SetCheckLocation(FVector(6400.0, -10150.0, -440.0));
		}

		SpawnMonsterData = IntermediateMonsterSpawn(PlLevel, FVector(11820.0, -10360.0, -440.0));
		AMMMonsterArea* Num4MonsterArea = GetWorld()->SpawnActor<AMMMonsterArea>(AMMMonsterArea::StaticClass());
		for (int i = 0; i < SpawnMonsterData.Num(); i++)
		{
			SpawnMonster.Add(SpawnMonsterData[i]);
			Num4MonsterArea->AddMonsterData(SpawnMonsterData[i]);
			Num4MonsterArea->SetCheckLocation(FVector(11820.0, -10360.0, -440.0));
		}

	}
}
void AMMMonsterFieldSpawner::SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


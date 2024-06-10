// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMGameMode.h"

#include "Monster/MMMonsterFieldSpawner.h"

AMMGameMode::AMMGameMode()
{
	// DefaultPawn 설정
	static ConstructorHelpers::FClassFinder<APawn> PawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Player/BP_Player.BP_Player_C'"));
	if (PawnClassRef.Succeeded())
	{
		DefaultPawnClass = PawnClassRef.Class;
	}

	// PlayerControllerClass 설정
	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Player/BP_MMPlayerController.BP_MMPlayerController_C'"));
	if (ControllerClassRef.Succeeded())
	{
		PlayerControllerClass = ControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMMonsterFieldSpawner> SpawnerClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/BP_MonsterFieldSpawner.BP_MonsterFieldSpawner_C'"));
	if (SpawnerClassRef.Succeeded())
	{
		SpawnerData = SpawnerClassRef.Class;
	}
}

void AMMGameMode::BeginPlay()
{
	Super::BeginPlay();

	Spawner = GetWorld()->SpawnActor<AMMMonsterFieldSpawner>(SpawnerData);
}

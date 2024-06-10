// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMGameMode.h"
#include "Monster/MMMonsterFieldSpawner.h"

#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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


	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->OnAudioFinished.AddDynamic(this, &AMMGameMode::OnSoundFinished);

	static ConstructorHelpers::FObjectFinder<USoundWave> SoundWaveAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/audio/Elven_Harmonies__no_percussion_.Elven_Harmonies__no_percussion_'"));
	if (SoundWaveAsset.Succeeded())
	{
		AudioComponent->SetSound(SoundWaveAsset.Object);
	}
}

void AMMGameMode::BeginPlay()
{
	Super::BeginPlay();

	Spawner = GetWorld()->SpawnActor<AMMMonsterFieldSpawner>(SpawnerData);

	if (AudioComponent && AudioComponent->GetSound())
	{
		AudioComponent->Play();
	}
}

void AMMGameMode::OnSoundFinished()
{
	if (AudioComponent && AudioComponent->GetSound())
	{
		AudioComponent->Play();
	}
}

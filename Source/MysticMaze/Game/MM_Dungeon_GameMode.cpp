// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MM_Dungeon_GameMode.h"

#include "Dungeon/MMRoomBase.h"
#include "UI/MM_Dungeon_UI.h"
#include "Game/MMGameInstance.h"
#include "Dungeon/MMPortal.h"
#include "Engine/GameInstance.h"
#include "UI/MMClearWidget.h"

#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AMM_Dungeon_GameMode::AMM_Dungeon_GameMode()
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

	static ConstructorHelpers::FClassFinder<AMMRoomBase> FirstRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Dungeon/First_Floor/BP_FirstStartRoom.BP_FirstStartRoom_C'"));
	if (FirstRef.Class)
	{
		FirstRoomData = FirstRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AMMRoomBase> SecondRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Dungeon/Second_Floor/BP_SecondStartRoom.BP_SecondStartRoom_C'"));
	if (SecondRef.Class)
	{
		SecondRoomData = SecondRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AMMRoomBase> ThirdRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Dungeon/Third_Floor/BP_ThirdStartRoom.BP_ThirdStartRoom_C'"));
	if (ThirdRef.Class)
	{
		ThirdRoomData = ThirdRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UMM_Dungeon_UI> UIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_DungoenClear.WBP_DungoenClear_C'"));
	if (UIRef.Class)
	{
		DungeongUIOrigin = UIRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMMPortal> PortalRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Dungeon/BP_Portal.BP_Portal_C'"));
	if (PortalRef.Class)
	{
		PortalOrigin = PortalRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UMMClearWidget> ClearRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MysticMaze/UI/WBP_ClearWidget.WBP_ClearWidget_C'"));
	if (ClearRef.Class)
	{
		DungeonClearOrigin = ClearRef.Class;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->OnAudioFinished.AddDynamic(this, &AMM_Dungeon_GameMode::OnSoundFinished);

	static ConstructorHelpers::FObjectFinder<USoundWave> SoundWaveAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/audio/Fantasy_Adventure.Fantasy_Adventure'"));
	if (SoundWaveAsset.Succeeded())
	{
		AudioComponent->SetSound(SoundWaveAsset.Object);
	}

	SpawnStartRoom = 0;
}

void AMM_Dungeon_GameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	SpawnStartRoom = 1;

	if (DungeongUIOrigin)
	{
		DungeongUI = CreateWidget<UMM_Dungeon_UI>(GetWorld(), DungeongUIOrigin);
		if (DungeongUI)
		{
			DungeongUI->Init();
			DungeongUI->AddToViewport();
		}
	}
}

void AMM_Dungeon_GameMode::SetRoomCount(FVector INData)
{
	RoomCount++;
	if (RoomCount >= 5)
	{
		RoomCount = 5;

		FVector PortalSpawnLocation = INData;
		PortalSpawnLocation.Z += 400;
		FTransform Transform;
		Transform.SetLocation(PortalSpawnLocation);

		

		UMMGameInstance* GameInstanceData = Cast<UMMGameInstance>(GetGameInstance());
		if (GameInstanceData)
		{
			int32 FloorData = GameInstanceData->GetCurrentFloor();
			FloorData++;
			if (FloorData < 3)
			{
				AMMPortal* RoomPortal = GetWorld()->SpawnActor<AMMPortal>(PortalOrigin, Transform);
			}
			else if (FloorData >= 4)
			{
				FloorData = 3;

				if (DungeonClearOrigin)
				{
					DungeonClear = CreateWidget<UMMClearWidget>(GetWorld(), DungeonClearOrigin);
					if (DungeonClear)
					{
						DungeonClear->AddToViewport();
					}
				}
			}

			GameInstanceData->SetCurrentFloor(FloorData);
		}

	}

	if (DungeongUI)
	{
		DungeongUI->UpdateText();
	}
}

void AMM_Dungeon_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComponent && AudioComponent->GetSound())
	{
		AudioComponent->Play();
	}

	RoomCount = 0;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0, 0, 1000));
	
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		SpawnStartRoom = GameInstance->GetCurrentFloor();
	}
	
	switch (SpawnStartRoom)
	{
	case 0:
		break;

	case 1:
		GetWorld()->SpawnActor<AMMRoomBase>(FirstRoomData, SpawnTransform);
		break;

	case 2:
		GetWorld()->SpawnActor<AMMRoomBase>(SecondRoomData, SpawnTransform);
		break;

	case 3:
		GetWorld()->SpawnActor<AMMRoomBase>(ThirdRoomData, SpawnTransform);
		break;
	}
	
}

void AMM_Dungeon_GameMode::OnSoundFinished()
{
	if (AudioComponent && AudioComponent->GetSound())
	{
		AudioComponent->Play();
	}
}
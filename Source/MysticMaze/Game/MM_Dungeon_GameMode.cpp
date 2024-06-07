// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MM_Dungeon_GameMode.h"

#include "Dungeon/MMRoomBase.h"
#include "UI/MM_Dungeon_UI.h"

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

	SpawnStartRoom = 0;


	
}

void AMM_Dungeon_GameMode::SetRoomCount()
{
	RoomCount++;
	if (DungeongUI)
	{
		DungeongUI->UpdateText();
	}
}

void AMM_Dungeon_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (DungeongUIOrigin)
	{
		DungeongUI = CreateWidget<UMM_Dungeon_UI>(GetWorld(), DungeongUIOrigin);
		if (DungeongUI)
		{
			DungeongUI->Init();
			DungeongUI->AddToViewport();
		}
	}
	
	RoomCount = 0;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0, 0, 1000));

	SpawnStartRoom = 1;
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

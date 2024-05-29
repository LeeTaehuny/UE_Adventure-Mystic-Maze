// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMGameMode.h"

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
}

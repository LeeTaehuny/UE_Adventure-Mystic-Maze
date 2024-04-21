// Copyright Epic Games, Inc. All Rights Reserved.

#include "MysticMazeGameMode.h"
#include "MysticMazeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMysticMazeGameMode::AMysticMazeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

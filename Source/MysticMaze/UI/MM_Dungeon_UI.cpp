// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MM_Dungeon_UI.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MM_Dungeon_GameMode.h"

void UMM_Dungeon_UI::Init()
{
	UpdateText();
}

void UMM_Dungeon_UI::UpdateText()
{
	AMM_Dungeon_GameMode* GameMode = Cast<AMM_Dungeon_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		FString strText;
		strText = FString::Printf(TEXT("%d"), GameMode->GetRoomCount());

		TXT_Number->SetText(FText::FromString(strText));
	}
}

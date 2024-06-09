// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMGameInstance.h"
#include "Game/MMSaveGameData.h"
#include "GameData/MMSaveInitData.h"

#include "Kismet/GameplayStatics.h"

UMMGameInstance::UMMGameInstance()
{
	// 플레이어 레벨 데이터 테이블을 불러와 레벨 테이블에 저장합니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/MysticMaze/DataTable/Player/DT_PlayerStatusData.DT_PlayerStatusData'"));
	if (DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		Algo::Transform(ValueArray, PlayerStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FMMCharacterStat*>(Value);
			}
		);
	}
	MaxLevel = PlayerStatTable.Num();

	// Save
	static ConstructorHelpers::FObjectFinder<UMMSaveInitData> SaveInitDataRef(TEXT("/Script/MysticMaze.MMSaveInitData'/Game/MysticMaze/Game/DA_StartData.DA_StartData'"));
	if (SaveInitDataRef.Object)
	{
		InItData = SaveInitDataRef.Object;
	}
	SaveSlotName = TEXT("SaveSlot");
}

void UMMGameInstance::Exit()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 게임 종료
		PlayerController->ConsoleCommand("quit");
	}
}

void UMMGameInstance::SetSaveSlot(int32 InIndex)
{
	SaveSlotName += FString::FromInt(InIndex);
	UE_LOG(LogTemp, Warning, TEXT("Save %s"), *SaveSlotName);

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveDataInstance = Cast<UMMSaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		CurrentFloor = SaveDataInstance->CurrentFloor;
	}
	else
	{
		SaveDataInstance = Cast<UMMSaveGameData>(UGameplayStatics::CreateSaveGameObject(UMMSaveGameData::StaticClass()));

		SaveDataInstance->SaveSlotName = SaveSlotName;
		SaveDataInstance->SaveIndex = 0;
		SaveDataInstance->Level = InItData->Level;
		SaveDataInstance->Gold = InItData->Gold;
		SaveDataInstance->Class = InItData->Class;
		SaveDataInstance->CurrentFloor = InItData->CurrentFloor;
		CurrentFloor = SaveDataInstance->CurrentFloor;
		UGameplayStatics::SaveGameToSlot(SaveDataInstance, SaveDataInstance->SaveSlotName, SaveDataInstance->SaveIndex);
	}
}

void UMMGameInstance::SaveDungeonFloor()
{
	if (SaveDataInstance)
	{
		SaveDataInstance->CurrentFloor = CurrentFloor;
	}
}
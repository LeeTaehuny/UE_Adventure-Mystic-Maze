// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MM_Dungeon_GameMode.h"

AMM_Dungeon_GameMode::AMM_Dungeon_GameMode()
{
	// �÷��̾� ���� ������ ���̺��� �ҷ��� ���� ���̺� �����մϴ�.
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
}

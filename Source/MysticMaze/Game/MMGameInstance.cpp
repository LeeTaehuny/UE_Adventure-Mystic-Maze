// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MMGameInstance.h"

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
}

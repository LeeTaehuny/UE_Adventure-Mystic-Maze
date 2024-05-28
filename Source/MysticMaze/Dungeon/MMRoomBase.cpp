// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMRoomBase.h"


#include "Components/BrushComponent.h"

#include "AI/NavigationSystemBase.h"
#include "Engine/World.h"
#include "NavMesh/RecastNavMesh.h"

#include "Monster/MMMonsterSpawner.h"


// Sets default values
AMMRoomBase::AMMRoomBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bMonsterAlive = true;
	bClear = false;
	bFirstContact = false;
	bDoorRock = false;

	CurDoorUp = 0;
}

bool AMMRoomBase::SpawnNrothRoom(FVector INCenterLocation)
{
	if (World)
	{
		// 충돌 범위 설정
		FVector CheckCollision = FVector(150, 150, 500);

		// 1번 위치에 좌표 고정
		TArray<FHitResult> HitResults;
		FVector CenterLocation = INCenterLocation + FVector(0, RoomSize, 0);
		FVector SweepStart = CenterLocation - CheckCollision;
		FVector SweepEnd = CenterLocation + CheckCollision;
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(10, 10, 10));

		/*
		3 4 5
		0 1 2
			6
		*/
		bool Location0 = false;
		bool Location2 = false;
		bool Location3 = false;
		bool Location4 = false;
		bool Location5 = false;

		// 1번 위치 충돌체크 : 해당 위치에 충돌이 발생하면 더 계산할 필요 없음
		if (World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			return true;
		}

		// 0번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize, RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location0 = true;
		}

		// 2번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize, RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location2 = true;
		}

		// 3번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize, RoomSize * 2.0f, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location3 = true;
		}

		// 4번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(0, RoomSize * 2.0f, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location4 = true;
		}

		// 5번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize, RoomSize * 2.0f, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location5 = true;
		}

		bool Location6 = false;
		if (Location2)
		{
			CenterLocation = INCenterLocation + FVector(-RoomSize, 0, 0);
			SweepStart = CenterLocation - CheckCollision;
			SweepEnd = CenterLocation + CheckCollision;
			if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
			{
				Location6 = true;
			}
		}

		/*
		0 : 1칸
		1 : 2칸 직선
		2 : 3칸 ㄱ
		3 : 3칸 ㄴ

		3 4 5
		0 1 2
			6
		*/
		TArray<int> Excluded_Numbers;
		// 0번과 2번 위치에 무언가가 있다면 직선 룸, ㄴ룸 스폰 불가로 제외
		if (!Location0 && !Location2)
		{
			Excluded_Numbers.Add(1);
			Excluded_Numbers.Add(3);

			// 4번, 5번, 룸이 있을 경우 ㄱ룸 스폰 불가로 제외
			// 6번의 경우 이미 2번이 제외되었기 때문에 검사할 필요도 없음, 어차피 스폰 불가
			if (!Location4 || !Location5)
			{
				Excluded_Numbers.Add(2);
			}
		}

		if ((!Location3 && !Location4))
		{
			bool onreal = false;
			for (int i = 0; i < Excluded_Numbers.Num(); i++)
			{
				if (Excluded_Numbers[i] == 2)
				{
					onreal = true;
				}
			}
			if (onreal)
			{
				Excluded_Numbers.Add(2);
			}
		}

		int SpawnRoomNumber = 0;
		while (true)
		{
			SpawnRoomNumber = FMath::RandRange(0, 3);
			bool same = false;

			for (int i = 0; i < Excluded_Numbers.Num(); i++)
			{
				if (SpawnRoomNumber == Excluded_Numbers[i])
				{
					same = true;
					break;
				}
			}

			if (!same)
			{
				break;
			}
		}

		AMMRoomBase* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(0, RoomSize, 0);
			aa = World->SpawnActor<AMMRoomBase>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 1:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0)
			{
				LocationReadJust = 2;
			}
			else if (!Location2)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 0.5f, RoomSize, 0);
			}
			aa = World->SpawnActor<AMMRoomBase>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 2:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location2 || !Location6)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, RoomSize * 2.0f, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 1.5f, RoomSize, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 3:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0 || !Location3)
			{
				LocationReadJust = 2;
			}
			else if (!Location2 || !Location4)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 0.5f, RoomSize, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		default:
			break;
		}
	}

	return false;
}
bool AMMRoomBase::SpawnSouthRoom(FVector INCenterLocation)
{
	if (World)
	{
		FVector CheckCollision = FVector(150, 150, 500);

		TArray<FHitResult> HitResults;
		FVector CenterLocation = INCenterLocation + FVector(0, -RoomSize, 0);
		FVector SweepStart = CenterLocation - CheckCollision;
		FVector SweepEnd = CenterLocation + CheckCollision;
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(10, 10, 10));

		/*
		6
		0 1 2
		3 4 5
		*/
		bool Location0 = false;
		bool Location2 = false;
		bool Location3 = false;
		bool Location4 = false;
		bool Location5 = false;
		bool Location6 = false;
		// 1번 위치 충돌체크 : 해당 위치에 충돌이 발생하면 더 계산할 필요 없음
		if (World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			return true;
		}

		// 0번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize, -RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location0 = true;
		}

		// 2번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize, -RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location2 = true;
		}

		// 3번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize, -RoomSize * 2.0f, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location3 = true;
		}

		// 4번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(0, -RoomSize * 2.0f, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location4 = true;
		}

		// 5번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize, -RoomSize * 2.0f, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location5 = true;
		}

		if (Location0)
		{
			CenterLocation = INCenterLocation + FVector(RoomSize, 0, 0);
			SweepStart = CenterLocation - CheckCollision;
			SweepEnd = CenterLocation + CheckCollision;
			if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
			{
				Location6 = true;
			}
		}

		/*
		0 : 1칸
		1 : 2칸 직선
		2 : 3칸 ㄱ
		3 : 3칸 ㄴ
		*/

		/*
		6
		0 1 2
		3 4 5
		*/
		TArray<int> Excluded_Numbers;
		if (!Location0 && !Location2)
		{
			Excluded_Numbers.Add(1);
			Excluded_Numbers.Add(2);
			if (!Location6)
			{
				Excluded_Numbers.Add(3);
			}
		}

		if ((!Location4 && !Location5))
		{
			bool onreal = false;
			for (int i = 0; i < Excluded_Numbers.Num(); i++)
			{
				if (Excluded_Numbers[i] == 3)
				{
					onreal = true;
				}
			}
			if (onreal)
			{
				Excluded_Numbers.Add(3);
			}
		}

		int SpawnRoomNumber = 0;
		while (true)
		{
			SpawnRoomNumber = FMath::RandRange(0, 3);
			bool same = false;

			for (int i = 0; i < Excluded_Numbers.Num(); i++)
			{
				if (SpawnRoomNumber == Excluded_Numbers[i])
				{
					same = true;
					break;
				}
			}

			if (!same)
			{
				break;
			}
		}

		AMMRoomBase* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(0, -RoomSize, 0);
			aa = World->SpawnActor<AMMRoomBase>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 1:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0)
			{
				LocationReadJust = 2;
			}
			else if (!Location2)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, -RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 0.5f, -RoomSize, 0);
			}
			aa = World->SpawnActor<AMMRoomBase>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 2:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0 || !Location4)
			{
				LocationReadJust = 2;
			}
			else if (!Location2 || !Location5)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, -RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 0.5f, -RoomSize, 0);
			}

			

			aa = World->SpawnActor<AMMRoomBase>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 3:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0 || !Location6)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 0.5f, -RoomSize * 2.0f, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, -RoomSize, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		default:
			break;
		}


	}

	return false;
}
bool AMMRoomBase::SpawnEastRoom(FVector INCenterLocation)
{
	if (World)
	{
		FVector CheckCollision = FVector(150, 150, 500);

		TArray<FHitResult> HitResults;
		FVector CenterLocation = INCenterLocation + FVector(-RoomSize, 0, 0);
		FVector SweepStart = CenterLocation - CheckCollision;
		FVector SweepEnd = CenterLocation + CheckCollision;
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(10, 10, 10));

		/*
		  0 3
		  1 4
		6 2 5
		*/
		bool Location0 = false;
		bool Location2 = false;
		bool Location3 = false;
		bool Location4 = false;
		bool Location5 = false;
		bool Location6 = false;
		// 1번 위치 충돌체크 : 해당 위치에 충돌이 발생하면 더 계산할 필요 없음
		if (World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			return true;
		}

		// 0번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize, RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location0 = true;
		}

		// 2번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize, -RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location2 = true;
		}

		// 3번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize * 2.0f, RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location3 = true;
		}

		// 4번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize * 2.0f, 0, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location4 = true;
		}

		// 5번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(-RoomSize * 2.0f, -RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location5 = true;
		}

		if (Location2)
		{
			CenterLocation = INCenterLocation + FVector(0, RoomSize, 0);
			SweepStart = CenterLocation - CheckCollision;
			SweepEnd = CenterLocation + CheckCollision;
			if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
			{
				Location6 = true;
			}
		}

		/*
		0 : 1칸
		1 : 2칸 직선
		2 : 3칸 ㄱ
		3 : 3칸 ㄴ
		*/
		TArray<int> Excluded_Numbers;
		if (!Location0 && !Location2)
		{
			Excluded_Numbers.Add(3);
		}

		if (!Location4)
		{
			Excluded_Numbers.Add(1);
			Excluded_Numbers.Add(2);
		}

		if (!Location5)
		{
			Excluded_Numbers.Add(2);
		}

		if (!Location6 && !Location0)
		{
			Excluded_Numbers.Add(2);
		}

		int SpawnRoomNumber = 0;
		while (true)
		{
			SpawnRoomNumber = FMath::RandRange(0, 3);
			bool same = false;

			for (int i = 0; i < Excluded_Numbers.Num(); i++)
			{
				if (SpawnRoomNumber == Excluded_Numbers[i])
				{
					same = true;
					break;
				}
			}

			if (!same)
			{
				break;
			}
		}

		AMMRoomBase* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(-RoomSize, 0, 0);
			aa = World->SpawnActor<AMMRoomBase>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 1:
			CenterLocation = INCenterLocation + FVector(-RoomSize * 1.5f, 0, 0);
			aa = World->SpawnActor<AMMRoomBase>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 2:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0 || !Location6)
			{
				LocationReadJust = 2;
			}
			else if (!Location4 || !Location5)
			{
				LocationReadJust = 1;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 0.5f, RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 1.5f, 0, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 3:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0 || !Location4)
			{
				LocationReadJust = 1;
			}
			if (!Location2 || !Location5)
			{
				LocationReadJust = 5;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 1.5f, 0, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(-RoomSize * 1.5f, -RoomSize, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		default:
			break;
		}


	}

	return false;
}
bool AMMRoomBase::SpawnWestRoom(FVector INCenterLocation)
{
	if (World)
	{
		FVector CheckCollision = FVector(150, 150, 500);

		TArray<FHitResult> HitResults;
		FVector CenterLocation = INCenterLocation + FVector(RoomSize, 0, 0);
		FVector SweepStart = CenterLocation - CheckCollision;
		FVector SweepEnd = CenterLocation + CheckCollision;
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(10, 10, 10));

		/*
		5 2
		4 1
		3 0 6
		*/
		bool Location0 = false;
		bool Location2 = false;
		bool Location3 = false;
		bool Location4 = false;
		bool Location5 = false;
		bool Location6 = false;
		// 1번 위치 충돌체크 : 해당 위치에 충돌이 발생하면 더 계산할 필요 없음
		if (World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			return true;
		}

		// 0번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize, -RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location0 = true;
		}

		// 2번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize, RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location2 = true;
		}

		// 3번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize * 2.0f, -RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location3 = true;
		}

		// 4번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize * 2.0f, 0, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location4 = true;
		}

		// 5번 위치 충돌체크
		CenterLocation = INCenterLocation + FVector(RoomSize * 2.0f, RoomSize, 0);
		SweepStart = CenterLocation - CheckCollision;
		SweepEnd = CenterLocation + CheckCollision;
		if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
		{
			Location5 = true;
		}

		if (Location2)
		{
			CenterLocation = INCenterLocation + FVector(0, -RoomSize, 0);
			SweepStart = CenterLocation - CheckCollision;
			SweepEnd = CenterLocation + CheckCollision;
			if (!World->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, CollisionShape))
			{
				Location6 = true;
			}
		}

		/*
		0 : 1칸
		1 : 2칸 직선
		2 : 3칸 ㄱ
		3 : 3칸 ㄴ
		*/
		TArray<int> Excluded_Numbers;
		if (!Location2 && !Location0)
		{
			Excluded_Numbers.Add(2);
		}

		if (!Location4)
		{
			Excluded_Numbers.Add(1);
		}

		if ((!Location4 && !Location5) || (!Location0 && !Location6))
		{
			Excluded_Numbers.Add(3);
		}

		int SpawnRoomNumber = 0;
		while (true)
		{
			SpawnRoomNumber = FMath::RandRange(0, 3);
			bool same = false;

			for (int i = 0; i < Excluded_Numbers.Num(); i++)
			{
				if (SpawnRoomNumber == Excluded_Numbers[i])
				{
					same = true;
					break;
				}
			}

			if (!same)
			{
				break;
			}
		}

		AMMRoomBase* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(RoomSize, 0, 0);
			aa = World->SpawnActor<AMMRoomBase>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 1:
			CenterLocation = INCenterLocation + FVector(RoomSize * 1.5f, 0, 0);
			aa = World->SpawnActor<AMMRoomBase>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 2:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location0 || !Location4)
			{
				LocationReadJust = 1;
			}
			else if (!Location2 || !Location5)
			{
				LocationReadJust = 2;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 1.5f, RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 1.5f, 0, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		case 3:
			LocationReadJust = FMath::RandRange(1, 2);

			if (!Location5 || !Location4)
			{
				LocationReadJust = 1;
			}
			if (!Location0 || !Location6)
			{
				LocationReadJust = 2;
			}

			if (LocationReadJust == 1)
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 0.5f, -RoomSize, 0);
			}
			else
			{
				CenterLocation = INCenterLocation + FVector(RoomSize * 1.5f, 0, 0);
			}

			aa = World->SpawnActor<AMMRoomBase>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			aa->SetSpawner(Spawner);
			return true;

		default:
			break;
		}
	}

	return false;
}

void AMMRoomBase::RoomBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 룸에 처음 입장했을때 발동, 이곳에서 몬스터 스폰도 진행될 예정

	if (!bFirstContact)
	{
		bFirstContact = true;

		FVector LocalLocation = this->GetActorLocation();
		//NavMeshBoundsVolume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass());
		//if (NavMeshBoundsVolume)
		//{
		//	NavMeshBoundsVolume->GetRootComponent()->SetMobility(EComponentMobility::Movable);
		//	UBrushComponent* BrushComponent = Cast<UBrushComponent>(NavMeshBoundsVolume->GetComponentByClass(UBrushComponent::StaticClass()));
		//	if (BrushComponent)
		//	{
		//		BrushComponent->SetMobility(EComponentMobility::Movable);
		//	}
		//
		//	NavMeshBoundsVolume->GetRootComponent()->SetWorldLocation(LocalLocation);
		//	NavMeshBoundsVolume->GetRootComponent()->SetWorldScale3D(FVector(8000, 8000, 100));
		//	UNavigationSystemV1::GetCurrent(World)->OnNavigationBoundsUpdated(NavMeshBoundsVolume);
		//}
		//
		//if (FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
		//{
		//	FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->Build();
		//}

		// 몬스터 스포너를 스폰하기 위한 함수
		// 매개변수 : 액터의 원점 로케이션 정보 전달
		//SpawnerSumon(LocalLocation);

		SpawnType RnadomValue = GetRandomEnumValue();

		//RnadomValue = SpawnType::MechaniteOnly;
		switch (RoomType)
		{
		case 0:

			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation);
			break;

		case 1:
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(-2000, 0, 0), LocalLocation);
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(2000, 0, 0), LocalLocation);
			break;

		case 2:
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(-2000, 0, 0), LocalLocation);
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(2000, 0, 0), LocalLocation);
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(-2000, -4000, 0), LocalLocation);
			break;

		case 3:
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(-2000, 0, 0), LocalLocation);
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(2000, 0, 0), LocalLocation);
			Spawner->MonsterSpawn(RnadomValue, 1, LocalLocation + FVector(2000, 4000, 0), LocalLocation);
			break;

		default:
			break;
		}
	}
	
}

void AMMRoomBase::DoorUpDown(uint8 INSwitch, UStaticMeshComponent* INWallData)
{
	// 문을 내리기 위한 이프문
	if (INSwitch)
	{
		FVector location = INWallData->GetComponentLocation();
		INWallData->SetWorldLocation(FMath::Lerp(location, FVector(location.X, location.Y, LowZ), Alpha));
	}
	// 문을 올리기 위한 이프문
	else
	{
		FVector location = INWallData->GetComponentLocation();
		INWallData->SetWorldLocation(FMath::Lerp(location, FVector(location.X, location.Y, HighZ), Alpha));

		// 현재 문의 위치가 (최대높이 - 1) 보다 크거나 같고, 몬스터가 한 번 스폰되었다면
		if (location.Z >= HighZ - 0.5f && bFirstContact)
		{
			// 현재 닫힌 문의 수를 카운트하고
			CurDoorUp++;

			// 모든 문이 닫혀있다면 문을 잠금
			if (CurDoorUp >= MaxDoorUp)
			{
				bDoorRock = true;
			}
		}
	}
}

void AMMRoomBase::ClearSignal()
{
	// 몬스터가 모두 죽었을 경우, 몬스터의 생존 함수를 bool로 변경하고
	// 클리어 했다는 것을 알림
	bMonsterAlive = false;
	bClear = true;
}


/*
void AMMRoomBase::SpawnerSumon(FVector INLocation)
{
	AActor* MyActor;
	MyActor = GetWorld()->SpawnActorDeferred<AActor>(MonsterSpawner, FTransform(FVector()));
	if (MyActor)
	{
		// 받아온 블루프린트의 태그에 따라 분류
		// 1층 태그라면 1층 스포너를
		// 2층이라면 2층 스포너를
		// 3층이라면 3층을
		// 스폰한 스포너에 원점이 되는 함수 설정
		if (MyActor->ActorHasTag("FirstFloorSpawner"))
		{
			AMMFirstFloorMonsterSpawner* FirstFloor = Cast<AMMFirstFloorMonsterSpawner>(MyActor);
			if (FirstFloor)
			{
				// 룸의 타입에 따라 전달하는 위치 정보가 다름
				switch (RoomType)
				{
				case 0:
					FirstFloor->SetSpawnPoint(INLocation);
					break;

				case 1:
					FirstFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					FirstFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					break;

				case 2:
					FirstFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					FirstFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					FirstFloor->SetSpawnPoint(INLocation + FVector(-2000, -4000, 0));
					break;

				case 3:
					FirstFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					FirstFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					FirstFloor->SetSpawnPoint(INLocation + FVector(2000, 4000, 0));
					break;
				}
			}
		}
		else if (MyActor->ActorHasTag("SecondFloorSpawner"))
		{
			AMMSecondFloorMOnsterSpawner* SecondFloor = Cast<AMMSecondFloorMOnsterSpawner>(MyActor);
			if (SecondFloor)
			{
				switch (RoomType)
				{
				case 0:
					SecondFloor->SetSpawnPoint(INLocation);
					break;

				case 1:
					SecondFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					SecondFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					break;

				case 2:
					SecondFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					SecondFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					SecondFloor->SetSpawnPoint(INLocation + FVector(-2000, -4000, 0));
					break;

				case 3:
					SecondFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					SecondFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					SecondFloor->SetSpawnPoint(INLocation + FVector(2000, 4000, 0));
					break;
				}
			}
		}
		else if (MyActor->ActorHasTag("ThirdFloorSpawner"))
		{
			AMMThirdFloorMonsterSpawner* ThirdFloor = Cast<AMMThirdFloorMonsterSpawner>(MyActor);
			if (ThirdFloor)
			{
				switch (RoomType)
				{
				case 0:
					ThirdFloor->SetSpawnPoint(INLocation);
					break;

				case 1:
					ThirdFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					ThirdFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					break;

				case 2:
					ThirdFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					ThirdFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					ThirdFloor->SetSpawnPoint(INLocation + FVector(-2000, -4000, 0));
					break;

				case 3:
					ThirdFloor->SetSpawnPoint(INLocation + FVector(2000, 0, 0));
					ThirdFloor->SetSpawnPoint(INLocation + FVector(-2000, 0, 0));
					ThirdFloor->SetSpawnPoint(INLocation + FVector(2000, 4000, 0));
					break;
				}
			}
		}

		// 초기화 완료
		UGameplayStatics::FinishSpawningActor(MyActor, FTransform(FVector()));
	}
}
*/

SpawnType AMMRoomBase::GetRandomEnumValue()
{
	int32 EnumRange = static_cast<int32>(SpawnType::StrongGrux) + 1; // 마지막 Enum 값
	int32 RandomIndex = FMath::RandRange(0, EnumRange - 1);
	return static_cast<SpawnType>(RandomIndex);
}
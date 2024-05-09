// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMRoomBase.h"

// Sets default values
AMMRoomBase::AMMRoomBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMMRoomBase::SetCollision(UBoxComponent* InCollisoin, FName LInName, UStaticMeshComponent* InParent)
{
	InCollisoin = CreateDefaultSubobject<UBoxComponent>(TEXT(InName));
	if (InParent != NULL)
	{
		InCollisoin->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	BoxColliders.Add(InCollisoin);
}

void AMMRoomBase::SpawnNrothRoom(FVector INCenterLocation, bool& INRoomOn)
{
	if (INRoomOn)
	{
		return;
	}

	if (World)
	{
		FVector CheckCollision = FVector(150, 150, 500);

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
			INRoomOn = true;
			return;
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
		*/
		TArray<int> Excluded_Numbers;
		if (!Location0 && !Location2)
		{
			Excluded_Numbers.Add(1);
			Excluded_Numbers.Add(3);
			if (!Location6)
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
			SpawnRoomNumber = FMath::RandRange(0, 4);
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

		AActor* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(0, RoomSize, 0);
			aa = World->SpawnActor<AActor>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 1:
			LocationReadJust = FMath::RandRange(1, 3);

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
			aa = World->SpawnActor<AActor>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 2:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			
			break;

		case 3:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		default:
			break;
		}

		
	}
}
void AMMRoomBase::SpawnSouthRoom(FVector INCenterLocation, bool& INRoomOn)
{
	if (INRoomOn)
	{
		return;
	}

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
			INRoomOn = true;
			return;
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
			SpawnRoomNumber = FMath::RandRange(0, 4);
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

		AActor* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(0, -RoomSize, 0);
			aa = World->SpawnActor<AActor>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 1:
			LocationReadJust = FMath::RandRange(1, 3);

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
			aa = World->SpawnActor<AActor>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 2:
			LocationReadJust = FMath::RandRange(1, 3);

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

			

			aa = World->SpawnActor<AActor>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;

			break;

		case 3:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		default:
			break;
		}


	}
}
void AMMRoomBase::SpawnEastRoom(FVector INCenterLocation, bool& INRoomOn)
{
	if (INRoomOn)
	{
		return;
	}

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
			INRoomOn = true;
			return;
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
			SpawnRoomNumber = FMath::RandRange(0, 4);
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

		AActor* aa;
		int LocationReadJust = 0;
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(-RoomSize, 0, 0);
			aa = World->SpawnActor<AActor>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 1:
			CenterLocation = INCenterLocation + FVector(-RoomSize * 1.5f, 0, 0);
			aa = World->SpawnActor<AActor>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 2:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;

			break;

		case 3:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		default:
			break;
		}


	}
}
void AMMRoomBase::SpawnWestRoom(FVector INCenterLocation, bool& INRoomOn)
{
	if (INRoomOn)
	{
		return;
	}

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
			INRoomOn = true;
			return;
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
			SpawnRoomNumber = FMath::RandRange(0, 4);
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

		AActor* aa;
		int LocationReadJust = 0;
		if (RoomChois != -1)
		{
			SpawnRoomNumber = RoomChois;
		}
		switch (SpawnRoomNumber)
		{
		case 0:
			CenterLocation = INCenterLocation + FVector(RoomSize, 0, 0);
			aa = World->SpawnActor<AActor>(ATypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 1:
			CenterLocation = INCenterLocation + FVector(RoomSize * 1.5f, 0, 0);
			aa = World->SpawnActor<AActor>(BTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		case 2:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(CTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;

			break;

		case 3:
			LocationReadJust = FMath::RandRange(1, 3);

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

			aa = World->SpawnActor<AActor>(DTypeData);
			aa->SetActorLocation(CenterLocation);
			INRoomOn = true;
			break;

		default:
			break;
		}


	}
}

void AMMRoomBase::FirstBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!FirstContact && OtherActor->ActorHasTag(FName("Player")))
	{
		FirstContact = true;
	}
}

void AMMRoomBase::DoorUpDown(bool INSwitch, UStaticMeshComponent* INWallData)
{
	if (INSwitch)
	{
		FVector location = INWallData->GetComponentLocation();
		INWallData->SetWorldLocation(FMath::Lerp(location, FVector(location.X, location.Y, LowZ), Alpha));
	}
	else
	{
		FVector location = INWallData->GetComponentLocation();
		INWallData->SetWorldLocation(FMath::Lerp(location, FVector(location.X, location.Y, HighZ), Alpha));
	}
}

void AMMRoomBase::DoorRule(AActor* Actor, bool& INSwitch, bool& INNextRoom, int INCompass, FVector INCorrect)
{
	if (!Actor->ActorHasTag(FName("Player")))
	{
		return;
	}

	if (!MonsterAlive || !FirstContact)
	{
		INSwitch = true;
	}

	if (Clear)
	{
		INSwitch = true;

		if (!INNextRoom)
		{
			FVector CurLocation = this->GetActorLocation() + INCorrect;

			switch (INCompass)
			{
			case 1:
				AMMRoomBase::SpawnNrothRoom(CurLocation, INNextRoom);
				break;
			case 2:
				AMMRoomBase::SpawnWestRoom(CurLocation, INNextRoom);
				break;
			case 3:
				AMMRoomBase::SpawnEastRoom(CurLocation, INNextRoom);
				break;
			case 4:
				AMMRoomBase::SpawnSouthRoom(CurLocation, INNextRoom);
				break;
			}
		}
	}
}

void AMMRoomBase::DoorRock(UStaticMeshComponent* door)
{
	FVector location = door->GetComponentLocation();
	door->SetWorldLocation(FMath::Lerp(location, FVector(location.X, location.Y, HighZ), Alpha));
}



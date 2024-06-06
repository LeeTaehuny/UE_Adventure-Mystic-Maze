// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMRoom_Class_A.h"

AMMRoom_Class_A::AMMRoom_Class_A()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshRef(TEXT("/Engine/BasicShapes/Cube.Cube"));

	MainFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Floor"));
	if (WallMeshRef.Succeeded())
	{
		MainFloor->SetStaticMesh(WallMeshRef.Object);
	}
	RootComponent = MainFloor;

	// 룸의 중앙 콜리전 밑 부모 설정
	RoomCenter = CreateDefaultSubobject<UBoxComponent>(TEXT("Center"));
	RoomCenter->SetupAttachment(MainFloor);

	// 문 생성
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Door")));

	if (WallMeshRef.Succeeded())
	{	
		for (int i = 0; i < Wall.Num(); i++)
		{
			Wall[i]->SetStaticMesh(WallMeshRef.Object);
			Wall[i]->SetupAttachment(MainFloor);
		}
	}

	North = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision"));
	North->SetupAttachment(Wall[0]);
	BoxColliders.Add(North);

	East = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision"));
	East->SetupAttachment(Wall[1]);
	BoxColliders.Add(East);

	Weast = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision"));
	Weast->SetupAttachment(Wall[2]);
	BoxColliders.Add(Weast);

	South = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision"));
	South->SetupAttachment(Wall[3]);
	BoxColliders.Add(South);

	bNorth_Switch = false;
	bSouth_Switch = false;
	bWest_Switch = false;
	bEast_Switch = false;

	bNorth_Blocking = false;
	bSouth_Blocking = false;
	bWest_Blocking = false;
	bEast_Blocking = false;

	// 문의 개수
	// 지금 닫힌 문이 맥스도어업 값과 같거나 넘는다면 문 잠금,
	// 물론 한 번 몬스터가 스폰된 적이 있어야 함
	MaxDoorUp = 4;

	RoomType = 0;
}

// Called when the game starts or when spawned
void AMMRoom_Class_A::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	// 콜리전들에게 이벤트 바인딩
	North->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::NorthBeginOverlap);
	North->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::NorthEndOverlap);
	East->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::EastBeginOverlap);
	East->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::EastEndOverlap);
	Weast->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::WastBeginOverlap);
	Weast->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::WastEndOverlap);
	South->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::SouthBeginOverlap);
	South->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::SouthEndOverlap);

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::RoomBeginOverlap);

	Structure_Installation(this->GetActorLocation());
}

// Called every frame
void AMMRoom_Class_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 문의 연산을 줄이기 위한 도어록 uint8 : 1 변수
	if (!bDoorRock)
	{
		CurDoorUp = 0;
		DoorUpDown(bNorth_Switch, Wall[0]);
		DoorUpDown(bEast_Switch, Wall[1]);
		DoorUpDown(bWest_Switch, Wall[2]);
		DoorUpDown(bSouth_Switch, Wall[3]);
	}

	if (IsValid(MonsterArea))
	{
		if (MonsterArea->IfMonsterNull(DeltaTime))
		{
			ClearSignal();
			MonsterArea->Destroy();
			MonsterArea = nullptr;
		}
	}

}

void AMMRoom_Class_A::NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 문제의 콜리전에 충돌했을 경우 잠금 풀기
	if ((bMonsterAlive && !bFirstContact) ||
		(!bMonsterAlive && bFirstContact))
	{
		bDoorRock = false;
	}

	// 아직 몬스터가 스폰되지 않았다면 문 열음
	if (!bFirstContact || bNorth_Blocking)
	{
		bNorth_Switch = true;
		return;
	}

	// 해당 룸이 클리어되고 해당 방향에 다른 방이 없을 경우 문을 여는 것과 동시에 전방에 다른 룸 스폰
	if (bClear && !bNorth_Blocking)
	{
		bNorth_Switch = true;
		bNorth_Blocking = SpawnNrothRoom(this->GetActorLocation());
	}
}
void AMMRoom_Class_A::NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 문 닫기 잠그지는 않음
	bNorth_Switch = false;
}

void AMMRoom_Class_A::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((bMonsterAlive && !bFirstContact) ||
		(!bMonsterAlive && bFirstContact))
	{
		bDoorRock = false;
	}

	if (!bFirstContact || bWest_Blocking)
	{
		bWest_Switch = true;
		return;
	}

	if (bClear && !bWest_Blocking)
	{
		bWest_Switch = true;
		bWest_Blocking = SpawnWestRoom(this->GetActorLocation());
	}
}
void AMMRoom_Class_A::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bWest_Switch = false;
}

void AMMRoom_Class_A::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((bMonsterAlive && !bFirstContact) ||
		(!bMonsterAlive && bFirstContact))
	{
		bDoorRock = false;
	}

	if (!bEast_Blocking || bEast_Blocking)
	{
		bEast_Switch = true;
		return;
	}

	if (bClear && !bEast_Blocking)
	{
		bEast_Switch = true;
		bEast_Blocking = SpawnEastRoom(this->GetActorLocation());
	}
}
void AMMRoom_Class_A::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bEast_Switch = false;
}

void AMMRoom_Class_A::SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((bMonsterAlive && !bFirstContact) ||
		(!bMonsterAlive && bFirstContact))
	{
		bDoorRock = false;
	}

	if (!bFirstContact || bSouth_Blocking)
	{
		bSouth_Switch = true;
		return;
	}

	if (bClear && !bSouth_Blocking)
	{
		bSouth_Switch = true;
		bSouth_Blocking = SpawnSouthRoom(this->GetActorLocation());
	}
}
void AMMRoom_Class_A::SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSouth_Switch = false;
}



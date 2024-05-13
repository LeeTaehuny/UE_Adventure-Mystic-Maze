// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMFristStartRoom.h"

AMMFristStartRoom::AMMFristStartRoom()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshRef(TEXT("/Engine/BasicShapes/Cube.Cube"));

	MainFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Floor"));
	if (WallMeshRef.Succeeded())
	{
		MainFloor->SetStaticMesh(WallMeshRef.Object);
	}
	RootComponent = MainFloor;

	RoomCenter = CreateDefaultSubobject<UBoxComponent>(TEXT("Center"));
	RoomCenter->SetupAttachment(MainFloor);

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

	North_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision"));
	North_Collision->SetupAttachment(Wall[0]);
	BoxColliders.Add(North_Collision);

	East_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision"));
	East_Collision->SetupAttachment(Wall[1]);
	BoxColliders.Add(East_Collision);

	Wast_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision"));
	Wast_Collision->SetupAttachment(Wall[2]);
	BoxColliders.Add(Wast_Collision);

	South_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision"));
	South_Collision->SetupAttachment(Wall[3]);
	BoxColliders.Add(South_Collision);

	bNorth_Switch = false;
	bSouth_Switch = false;
	bWest_Switch = false;
	bEast_Switch = false;

	bNorth_Blocking = false;
	bSouth_Blocking = false;
	bWest_Blocking = false;
	bEast_Blocking = false;

	MaxDoorUp = 10;
}

void AMMFristStartRoom::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	North_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMFristStartRoom::NorthBeginOverlap);
	North_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMFristStartRoom::NorthEndOverlap);
	East_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMFristStartRoom::EastBeginOverlap);
	East_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMFristStartRoom::EastEndOverlap);
	Wast_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMFristStartRoom::WastBeginOverlap);
	Wast_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMFristStartRoom::WastEndOverlap);
	South_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMFristStartRoom::SouthBeginOverlap);
	South_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMFristStartRoom::SouthEndOverlap);

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::RoomBeginOverlap);
}

void AMMFristStartRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDoorRock)
	{
		CurDoorUp = 0;
		DoorUpDown(bNorth_Switch, Wall[0]);
		DoorUpDown(bEast_Switch, Wall[1]);
		DoorUpDown(bWest_Switch, Wall[2]);
		DoorUpDown(bSouth_Switch, Wall[3]);
	}	
}

void AMMFristStartRoom::NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

	if (!bFirstContact || bNorth_Blocking)
	{
		bNorth_Switch = true;
		return;
	}

	if (bClear && !bNorth_Blocking)
	{
		bNorth_Switch = true;
		bNorth_Blocking = SpawnNrothRoom(this->GetActorLocation());
	}
}
void AMMFristStartRoom::NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bNorth_Switch = false;
}

void AMMFristStartRoom::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

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
void AMMFristStartRoom::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bWest_Switch = false;
}

void AMMFristStartRoom::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

	if (!bFirstContact || bEast_Blocking)
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
void AMMFristStartRoom::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bEast_Switch = false;
}

void AMMFristStartRoom::SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

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
void AMMFristStartRoom::SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSouth_Switch = false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMFristStartRoom.h"
#include "MMFristStartRoom.h"

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
	RoomCenter->AttachToComponent(MainFloor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Door")));

	if (WallMeshRef.Succeeded())
	{
		for (int i = 0; i < Wall.Num(); i++)
		{
			Wall[i]->SetStaticMesh(WallMeshRef.Object);
			Wall[i]->AttachToComponent(MainFloor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

	North_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision"));
	North_Collision->AttachToComponent(Wall[0], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(North_Collision);

	East_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision"));
	East_Collision->AttachToComponent(Wall[1], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(East_Collision);

	Wast_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision"));
	Wast_Collision->AttachToComponent(Wall[2], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(Wast_Collision);

	South_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision"));
	South_Collision->AttachToComponent(Wall[3], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(South_Collision);

	for (int i = 0; i < 4; i++)
	{
		RoomOn.Add(false);
	}
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

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::FirstBeginOverlap);
}

void AMMFristStartRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MonsterAlive)
	{
		Clear = true;
	}

	if (FirstContact && MonsterAlive)
	{
		North_Switch = false;
		West_Switch = false;
		East_Switch = false;
		South_Switch = false;
	}

	DoorUpDown(North_Switch, Wall[0]);
	DoorUpDown(East_Switch, Wall[1]);
	DoorUpDown(West_Switch, Wall[2]);
	DoorUpDown(South_Switch, Wall[3]);
}

void AMMFristStartRoom::NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, North_Switch, RoomOn[0], 1);
}
void AMMFristStartRoom::NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	North_Switch = false;
}

void AMMFristStartRoom::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, West_Switch, RoomOn[1], 2);
}
void AMMFristStartRoom::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	West_Switch = false;
}

void AMMFristStartRoom::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, East_Switch, RoomOn[2], 3);
}
void AMMFristStartRoom::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	East_Switch = false;
}

void AMMFristStartRoom::SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, South_Switch, RoomOn[3], 4);
}
void AMMFristStartRoom::SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	South_Switch = false;
}

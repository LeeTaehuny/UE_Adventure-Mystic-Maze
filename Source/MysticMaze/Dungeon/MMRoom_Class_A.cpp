// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMRoomBase.h"
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

	North = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision"));
	North->AttachToComponent(Wall[0], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(North);

	East = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision"));
	East->AttachToComponent(Wall[1], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(East);

	Weast = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision"));
	Weast->AttachToComponent(Wall[2], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(Weast);

	South = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision"));
	South->AttachToComponent(Wall[3], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(South);

	bNorth_Switch = false;
	bSouth_Switch = false;
	bWest_Switch = false;
	bEast_Switch = false;

	bNorth_Blocking = false;
	bSouth_Blocking = false;
	bWest_Blocking = false;
	bEast_Blocking = false;
}

// Called when the game starts or when spawned
void AMMRoom_Class_A::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	North->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::NorthBeginOverlap);
	North->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::NorthEndOverlap);
	East->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::EastBeginOverlap);
	East->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::EastEndOverlap);
	Weast->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::WastBeginOverlap);
	Weast->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::WastEndOverlap);
	South->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_A::SouthBeginOverlap);
	South->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_A::SouthEndOverlap);

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::FirstBeginOverlap);
}

// Called every frame
void AMMRoom_Class_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMonsterAlive)
	{
		bClear = true;
	}

	if (bFirstContact && bMonsterAlive)
	{
		bNorth_Switch = false;
		bWest_Switch = false;
		bEast_Switch = false;
		bSouth_Switch = false;
	}
	
	DoorUpDown(bNorth_Switch, Wall[0]);
	DoorUpDown(bEast_Switch, Wall[1]);
	DoorUpDown(bWest_Switch, Wall[2]);
	DoorUpDown(bSouth_Switch, Wall[3]);
}

void AMMRoom_Class_A::NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
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
void AMMRoom_Class_A::NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bNorth_Switch = false;
}

void AMMRoom_Class_A::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
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
	if (!bEast_Blocking)
	{
		bEast_Switch = true;
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
	if (!bFirstContact)
	{
		bSouth_Switch = true;
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



// Fill out your copyright notice in the Description page of Project Settings.

#include "Dungeon/MMRoom_Class_B.h"

AMMRoom_Class_B::AMMRoom_Class_B()
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

	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Door_0")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Door_1")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Door_0")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Door_1")));

	if (WallMeshRef.Succeeded())
	{
		for (int i = 0; i < Wall.Num(); i++)
		{
			Wall[i]->SetStaticMesh(WallMeshRef.Object);
			Wall[i]->AttachToComponent(MainFloor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

	North_0 = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision_0"));
	North_0->AttachToComponent(Wall[0], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(North_0);

	North_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision_1"));
	North_1->AttachToComponent(Wall[1], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(North_1);

	East = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision"));
	East->AttachToComponent(Wall[2], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(East);

	Weast = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision"));
	Weast->AttachToComponent(Wall[3], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(Weast);

	South_0 = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision_0"));
	South_0->AttachToComponent(Wall[4], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(South_0);

	South_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision_1"));
	South_1->AttachToComponent(Wall[5], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(South_1);

	bNorth_Switch_0 = false;
	bNorth_Switch_1 = false;
	bSouth_Switch_0 = false;
	bSouth_Switch_1 = false;
	bWest_Switch = false;
	bEast_Switch = false;

	bNorth_Blocking_0 = false;
	bNorth_Blocking_1 = false;
	bSouth_Blocking_0 = false;
	bSouth_Blocking_1 = false;
	bWest_Blocking = false;
	bEast_Blocking = false;

	MaxDoorUp = 6;
}

// Called when the game starts or when spawned
void AMMRoom_Class_B::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	North_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthBeginOverlap_0);
	North_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthEndOverlap_0);
	North_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthBeginOverlap_1);
	North_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthEndOverlap_1);
	East->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::EastBeginOverlap);
	East->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::EastEndOverlap);
	Weast->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::WastBeginOverlap);
	Weast->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::WastEndOverlap);
	South_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthBeginOverlap_0);
	South_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthEndOverlap_0);
	South_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthBeginOverlap_1);
	South_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthEndOverlap_1);

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::RoomBeginOverlap);
}

// Called every frame
void AMMRoom_Class_B::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDoorRock)
	{
		CurDoorUp = 0;
		DoorUpDown(bNorth_Switch_0, Wall[0]);
		DoorUpDown(bNorth_Switch_1, Wall[1]);
		DoorUpDown(bEast_Switch, Wall[2]);
		DoorUpDown(bWest_Switch, Wall[3]);
		DoorUpDown(bSouth_Switch_0, Wall[4]);
		DoorUpDown(bSouth_Switch_1, Wall[5]);
	}
}

void AMMRoom_Class_B::NorthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

	if (!bFirstContact || bNorth_Blocking_0)
	{
		bNorth_Switch_0 = true;
		return;
	}

	if (bClear && !bNorth_Blocking_0)
	{
		bNorth_Switch_0 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 0, 0);
		bNorth_Blocking_0 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_B::NorthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bNorth_Switch_0 = false;
}


void AMMRoom_Class_B::NorthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

	if (!bFirstContact || bNorth_Blocking_1)
	{
		bNorth_Switch_1 = true;
		return;
	}

	if (bClear && !bNorth_Blocking_1)
	{
		bNorth_Switch_1 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(-2000, 0, 0);
		bNorth_Blocking_1 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_B::NorthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bNorth_Switch_1 = false;
}


void AMMRoom_Class_B::SouthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

	if (!bFirstContact || bSouth_Blocking_0)
	{
		bSouth_Switch_0 = true;
		return;
	}

	if (bClear && !bSouth_Blocking_0)
	{
		bSouth_Switch_0 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 0, 0);
		bSouth_Blocking_0 = SpawnSouthRoom(CurLocation);
	}
}
void AMMRoom_Class_B::SouthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSouth_Switch_0 = false;
}


void AMMRoom_Class_B::SouthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDoorRock = false;

	if (!bFirstContact || bSouth_Blocking_1)
	{
		bSouth_Switch_1 = true;
		return;
	}

	if (bClear && !bSouth_Blocking_1)
	{
		bSouth_Switch_1 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(-2000, 0, 0);
		bSouth_Blocking_1 = SpawnSouthRoom(CurLocation);
	}
}
void AMMRoom_Class_B::SouthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSouth_Switch_1 = false;
}


void AMMRoom_Class_B::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 0, 0);
		bWest_Blocking = SpawnWestRoom(CurLocation);
	}
}
void AMMRoom_Class_B::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bWest_Switch = false;
}


void AMMRoom_Class_B::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		FVector CurLocation = this->GetActorLocation() + FVector(-2000, 0, 0);
		bEast_Blocking = SpawnEastRoom(CurLocation);
	}
}
void AMMRoom_Class_B::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bEast_Switch = false;
}



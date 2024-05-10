// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMRoom_Class_D.h"

AMMRoom_Class_D::AMMRoom_Class_D()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshRef(TEXT("/Engine/BasicShapes/Cube.Cube"));

	MainFloor1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Floor_0"));
	if (WallMeshRef.Succeeded())
	{
		MainFloor1->SetStaticMesh(WallMeshRef.Object);
	}
	RootComponent = MainFloor1;

	RoomCenter = CreateDefaultSubobject<UBoxComponent>(TEXT("Center_0"));
	RoomCenter->AttachToComponent(MainFloor1, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	RoomCenter_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Center_1"));
	RoomCenter_1->AttachToComponent(MainFloor1, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North_0 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North_1 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East_0 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East_1 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West_0 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West_1 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South_0 Door")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South_1 Door")));

	if (WallMeshRef.Succeeded())
	{
		for (int i = 0; i < Wall.Num(); i++)
		{
			Wall[i]->SetStaticMesh(WallMeshRef.Object);
			Wall[i]->AttachToComponent(MainFloor1, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

	North_0 = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision_0"));
	North_0->AttachToComponent(Wall[0], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(North_0);

	North_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("North Collision_1"));
	North_1->AttachToComponent(Wall[1], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(North_1);

	East_0 = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision_0"));
	East_0->AttachToComponent(Wall[2], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(East_0);

	East_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("East Collision_1"));
	East_1->AttachToComponent(Wall[3], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(East_1);

	Weast_0 = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision_0"));
	Weast_0->AttachToComponent(Wall[4], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(Weast_0);

	Weast_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("West Collision_1"));
	Weast_1->AttachToComponent(Wall[5], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(Weast_1);

	South_0 = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision_0"));
	South_0->AttachToComponent(Wall[6], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(South_0);

	South_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("South Collision_1"));
	South_1->AttachToComponent(Wall[7], FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxColliders.Add(South_1);

	bNorth_Switch_0 = false;
	bNorth_Switch_1 = false;
	bSouth_Switch_0 = false;
	bSouth_Switch_1 = false;
	bWest_Switch_0 = false;
	bWest_Switch_1 = false;
	bEast_Switch_0 = false;
	bEast_Switch_1 = false;

	bNorth_Blocking_0 = false;
	bNorth_Blocking_1 = false;
	bSouth_Blocking_0 = false;
	bSouth_Blocking_1 = false;
	bWest_Blocking_0 = false;
	bWest_Blocking_1 = false;
	bEast_Blocking_0 = false;
	bEast_Blocking_1 = false;
}

// Called when the game starts or when spawned
void AMMRoom_Class_D::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	North_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::NorthBeginOverlap_0);
	North_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::NorthEndOverlap_0);

	North_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::NorthBeginOverlap_1);
	North_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::NorthEndOverlap_1);

	East_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::EastBeginOverlap_0);
	East_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::EastEndOverlap_0);

	East_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::EastBeginOverlap_1);
	East_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::EastEndOverlap_1);

	Weast_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::WastBeginOverlap_0);
	Weast_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::WastEndOverlap_0);

	Weast_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::WastBeginOverlap_1);
	Weast_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::WastEndOverlap_1);

	South_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::SouthBeginOverlap_0);
	South_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::SouthEndOverlap_0);

	South_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_D::SouthBeginOverlap_1);
	South_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_D::SouthEndOverlap_1);

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::FirstBeginOverlap);
	RoomCenter_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::FirstBeginOverlap);
}

// Called every frame
void AMMRoom_Class_D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMonsterAlive)
	{
		bClear = true;
	}

	if (bFirstContact && bMonsterAlive)
	{
		bNorth_Switch_0 = false;
		bNorth_Switch_1 = false;
		bEast_Switch_0 = false;
		bEast_Switch_1 = false;
		bWest_Switch_0 = false;
		bWest_Switch_1 = false;
		bSouth_Switch_0 = false;
		bSouth_Switch_1 = false;
	}

	DoorUpDown(bNorth_Switch_0, Wall[0]);
	DoorUpDown(bNorth_Switch_1, Wall[1]);
	DoorUpDown(bEast_Switch_0, Wall[2]);
	DoorUpDown(bEast_Switch_1, Wall[3]);
	DoorUpDown(bWest_Switch_0, Wall[4]);
	DoorUpDown(bWest_Switch_1, Wall[5]);
	DoorUpDown(bSouth_Switch_0, Wall[6]);
	DoorUpDown(bSouth_Switch_1, Wall[7]);
}

void AMMRoom_Class_D::NorthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bNorth_Switch_0 = true;
		return;
	}

	if (bClear && !bNorth_Blocking_0)
	{
		bNorth_Switch_0 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 4000, 0);
		bNorth_Blocking_0 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::NorthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bNorth_Switch_0 = false;
}

void AMMRoom_Class_D::NorthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
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
void AMMRoom_Class_D::NorthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bNorth_Switch_1 = false;
}

void AMMRoom_Class_D::SouthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bSouth_Switch_0 = true;
		return;
	}

	if (bClear && !bSouth_Blocking_0)
	{
		bSouth_Switch_0 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 0, 0);
		bSouth_Blocking_0 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::SouthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSouth_Switch_0 = false;
}

void AMMRoom_Class_D::SouthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bSouth_Switch_1 = true;
		return;
	}

	if (bClear && !bSouth_Blocking_1)
	{
		bSouth_Switch_1 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(-2000, 0, 0);
		bSouth_Blocking_1 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::SouthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSouth_Switch_1 = false;
}

void AMMRoom_Class_D::WastBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bWest_Switch_0 = true;
		return;
	}

	if (bClear && !bWest_Blocking_0)
	{
		bWest_Switch_0 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 4000, 0);
		bWest_Blocking_0 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::WastEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bWest_Switch_0 = false;
}

void AMMRoom_Class_D::WastBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bWest_Switch_1 = true;
		return;
	}

	if (bClear && !bWest_Blocking_1)
	{
		bWest_Switch_1 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 0, 0);
		bWest_Blocking_1 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::WastEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bWest_Switch_1 = false;
}

void AMMRoom_Class_D::EastBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bEast_Switch_0 = true;
		return;
	}

	if (bClear && !bEast_Blocking_0)
	{
		bEast_Switch_0 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(2000, 4000, 0);
		bEast_Blocking_0 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::EastEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bEast_Switch_0 = false;
}

void AMMRoom_Class_D::EastBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bFirstContact)
	{
		bEast_Switch_1 = true;
		return;
	}

	if (bClear && !bEast_Blocking_1)
	{
		bEast_Switch_1 = true;
		FVector CurLocation = this->GetActorLocation() + FVector(-2000, 0, 0);
		bEast_Blocking_1 = SpawnNrothRoom(CurLocation);
	}
}
void AMMRoom_Class_D::EastEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bEast_Switch_1 = false;
}

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

	for (int i = 0; i < 6; i++)
	{
		RoomOn.Add(false);
	}
}

// Called when the game starts or when spawned
void AMMRoom_Class_B::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	if (North_0)
	{
		North_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthBeginOverlap_0);
		North_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthEndOverlap_0);
	}

	if (North_1)
	{
		North_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthBeginOverlap_1);
		North_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::NorthEndOverlap_1);
	}

	if (East)
	{
		East->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::EastBeginOverlap);
		East->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::EastEndOverlap);
	}

	if (Weast)
	{
		Weast->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::WastBeginOverlap);
		Weast->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::WastEndOverlap);
	}

	if (South_0)
	{
		South_0->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthBeginOverlap_0);
		South_0->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthEndOverlap_0);
	}

	if (South_1)
	{
		South_1->OnComponentBeginOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthBeginOverlap_1);
		South_1->OnComponentEndOverlap.AddDynamic(this, &AMMRoom_Class_B::SouthEndOverlap_1);
	}

	RoomCenter->OnComponentBeginOverlap.AddDynamic(this, &AMMRoomBase::FirstBeginOverlap);
}

// Called every frame
void AMMRoom_Class_B::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MonsterAlive)
	{
		Clear = true;
	}

	if (FirstContact && MonsterAlive)
	{
		North_Switch_0 = false;
		North_Switch_1 = false;
		East_Switch = false;
		West_Switch = false;
		South_Switch_0 = false;
		South_Switch_1 = false;
	}

	DoorUpDown(North_Switch_0, Wall[0]);
	DoorUpDown(North_Switch_1, Wall[1]);
	DoorUpDown(East_Switch, Wall[2]);
	DoorUpDown(West_Switch, Wall[3]);
	DoorUpDown(South_Switch_0, Wall[4]);
	DoorUpDown(South_Switch_1, Wall[5]);
}

void AMMRoom_Class_B::NorthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, North_Switch_0, RoomOn[0], 1, FVector(2000, 0, 0));
}
void AMMRoom_Class_B::NorthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	North_Switch_0 = false;
}


void AMMRoom_Class_B::NorthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, North_Switch_1, RoomOn[1], 1, FVector(-2000, 0, 0));
}
void AMMRoom_Class_B::NorthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	North_Switch_1 = false;
}


void AMMRoom_Class_B::SouthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, South_Switch_0, RoomOn[4], 4, FVector(2000, 0, 0));
}
void AMMRoom_Class_B::SouthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	South_Switch_0 = false;
}


void AMMRoom_Class_B::SouthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, South_Switch_1, RoomOn[5], 4, FVector(-2000, 0, 0));
}
void AMMRoom_Class_B::SouthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	South_Switch_1 = false;
}


void AMMRoom_Class_B::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, West_Switch, RoomOn[3], 2, FVector(2000, 0, 0));
}
void AMMRoom_Class_B::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	West_Switch = false;
}


void AMMRoom_Class_B::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, East_Switch, RoomOn[2], 3, FVector(-2000, 0, 0));
}
void AMMRoom_Class_B::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	East_Switch = false;
}



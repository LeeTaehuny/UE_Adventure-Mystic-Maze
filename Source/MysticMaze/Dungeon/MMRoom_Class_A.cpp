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

	for (int i = 0; i < 4; i++)
	{
		RoomOn.Add(false);
	}

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

void AMMRoom_Class_A::NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, North_Switch, RoomOn[0], 1);
}
void AMMRoom_Class_A::NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	North_Switch = false;
}

void AMMRoom_Class_A::WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, West_Switch, RoomOn[1], 2);
}
void AMMRoom_Class_A::WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	West_Switch = false;
}

void AMMRoom_Class_A::EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, East_Switch, RoomOn[2], 3);
}
void AMMRoom_Class_A::EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	East_Switch = false;
}

void AMMRoom_Class_A::SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, South_Switch, RoomOn[3], 4);
}
void AMMRoom_Class_A::SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	South_Switch = false;
}



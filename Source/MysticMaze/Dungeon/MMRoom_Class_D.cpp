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

	for (int i = 0; i < 8; i++)
	{
		RoomOn.Add(false);
	}
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

	if (!MonsterAlive)
	{
		Clear = true;
	}

	if (FirstContact && MonsterAlive)
	{
		North_Switch_0 = false;
		North_Switch_1 = false;
		East_Switch_0 = false;
		East_Switch_1 = false;
		West_Switch_0 = false;
		West_Switch_1 = false;
		South_Switch_0 = false;
		South_Switch_1 = false;
	}

	DoorUpDown(North_Switch_0, Wall[0]);
	DoorUpDown(North_Switch_1, Wall[1]);
	DoorUpDown(East_Switch_0, Wall[2]);
	DoorUpDown(East_Switch_1, Wall[3]);
	DoorUpDown(West_Switch_0, Wall[4]);
	DoorUpDown(West_Switch_1, Wall[5]);
	DoorUpDown(South_Switch_0, Wall[6]);
	DoorUpDown(South_Switch_1, Wall[7]);
}

void AMMRoom_Class_D::NorthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, North_Switch_0, RoomOn[0], 1, FVector(2000, 4000, 0));
}
void AMMRoom_Class_D::NorthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	North_Switch_0 = false;
}

void AMMRoom_Class_D::NorthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, North_Switch_1, RoomOn[1], 1, FVector(-2000, 0, 0));
}
void AMMRoom_Class_D::NorthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	North_Switch_1 = false;
}

void AMMRoom_Class_D::SouthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, South_Switch_0, RoomOn[2], 4, FVector(2000, 0, 0));
}
void AMMRoom_Class_D::SouthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	South_Switch_0 = false;
}

void AMMRoom_Class_D::SouthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, South_Switch_1, RoomOn[3], 4, FVector(-2000, 0, 0));
}
void AMMRoom_Class_D::SouthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	South_Switch_1 = false;
}

void AMMRoom_Class_D::WastBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, West_Switch_0, RoomOn[4], 2, FVector(2000, 4000, 0));
}
void AMMRoom_Class_D::WastEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	West_Switch_0 = false;
}

void AMMRoom_Class_D::WastBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, West_Switch_1, RoomOn[5], 2, FVector(2000, 0, 0));
}
void AMMRoom_Class_D::WastEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	West_Switch_1 = false;
}

void AMMRoom_Class_D::EastBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, East_Switch_0, RoomOn[6], 3, FVector(2000, 4000, 0));
}
void AMMRoom_Class_D::EastEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	East_Switch_0 = false;
}

void AMMRoom_Class_D::EastBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMRoomBase::DoorRule(OtherActor, East_Switch_1, RoomOn[7], 3, FVector(-2000, 0, 0));
}
void AMMRoom_Class_D::EastEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	East_Switch_1 = false;
}

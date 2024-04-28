// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/MMFirstFloorStartRoom.h"

// Sets default values
AMMFirstFloorStartRoom::AMMFirstFloorStartRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CubePlant = 

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Plant"));
	//Wall1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Right"));

	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Right")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Left")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East Right")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East Left")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West Right")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West Left")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Right")));
	Wall.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Left")));

	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UMaterial* material = LoadObject<UMaterial>(nullptr, TEXT("UMaterial'/Game/Megascans/Surfaces/Desert_Outback_Ground_Mud_Rocky_01_xikxcgl/first_floor_lend.first_floor_lend'"));
	mesh->SetStaticMesh(CubeMesh);
	mesh->SetMaterial(0, material);
	mesh->SetWorldLocation(FVector(0, 0, 0));
	mesh->SetWorldScale3D(FVector(60, 60, 1));

	for (int i = 0; i < Wall.Num(); i++)
	{
		Wall[i]->AttachToComponent(mesh->GetAttachmentRoot(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		//UStaticMesh* cubemmmm = ;
		Wall[i]->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
		Wall[i]->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("UMaterial'/Game/Megascans/Surfaces/Desert_Outback_Ground_Mud_Rocky_01_xikxcgl/first_floor_lend.first_floor_lend'")));
	}
}

// Called when the game starts or when spawned
void AMMFirstFloorStartRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMMFirstFloorStartRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


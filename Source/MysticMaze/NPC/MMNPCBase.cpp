// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/MMNPCBase.h"
#include "Collision/MMCollision.h"

#include "Components/SphereComponent.h"

// Sets default values
AMMNPCBase::AMMNPCBase()
{
	// 상호작용 범위
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Collsion"));
	Trigger->InitSphereRadius(200.0f);
	Trigger->SetCollisionProfileName(MMTRIGGER);
	RootComponent = Trigger;

	// 상인 메쉬
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMMNPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}


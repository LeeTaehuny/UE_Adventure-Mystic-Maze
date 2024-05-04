// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMArrow.h"

#include "Components/SphereComponent.h"

AMMArrow::AMMArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	FeatherComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Feather"));
	FeatherComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FeatherComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	RootComponent = FeatherComponent;

	ShaftComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shaft"));
	ShaftComponent->SetupAttachment(FeatherComponent);
	ShaftComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IronComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Iron"));
	IronComponent->SetupAttachment(ShaftComponent);
	IronComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ArrowCollision"));
	ArrowCollision->SetupAttachment(IronComponent);
}

void AMMArrow::BeginPlay()
{
	Super::BeginPlay();

}

void AMMArrow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMMArrow::Fire()
{
	// TODO : 발사
}


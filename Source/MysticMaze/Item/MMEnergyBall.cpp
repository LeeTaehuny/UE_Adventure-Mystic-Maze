// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMEnergyBall.h"

// Sets default values
AMMEnergyBall::AMMEnergyBall()
{

}

// Called when the game starts or when spawned
void AMMEnergyBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMMEnergyBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMMEnergyBall::Fire(FVector TargetLocation)
{

}

void AMMEnergyBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}


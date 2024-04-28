// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMFirstFloorStartRoom.generated.h"

UCLASS()
class MYSTICMAZE_API AMMFirstFloorStartRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMFirstFloorStartRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent*> Wall;

	//UStaticMeshComponent* Wall1;
	/*
	0 = 북쪽 오른쪽
	1 = 북쪽 왼쪽
	2 = 동쪽 오른쪽
	3 = 동쪽 왼쪽
	4 = 서쪽 오른쪽
	5 = 서쪽 왼쪽
	6 = 남쪽 오른쪽
	7 = 남쪽 왼쪽
	*/

};

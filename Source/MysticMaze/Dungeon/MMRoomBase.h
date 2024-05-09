// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MMRoomBase.generated.h"

UCLASS()
class MYSTICMAZE_API AMMRoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMRoomBase();

	UFUNCTION()
	void SetCollision(UBoxComponent* InCollisoin, FName LInName, UStaticMeshComponent* InParent = NULL);

	UFUNCTION()
	void FirstBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnNrothRoom(FVector INCenterLocation, bool& INRoomOn);
	UFUNCTION()
	void SpawnSouthRoom(FVector INCenterLocation, bool& INRoomOn);
	UFUNCTION()
	void SpawnEastRoom(FVector INCenterLocation, bool& INRoomOn);
	UFUNCTION()
	void SpawnWestRoom(FVector INCenterLocation, bool& INRoomOn);

	UFUNCTION()
	void DoorUpDown(bool INSwitch, UStaticMeshComponent* INWallData);

	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	int RoomChois = -1;

	

	UFUNCTION()
	void DoorRule(AActor* Actor, bool& INSwitch, bool& INNextRoom, int INCompass, FVector INCorrect = FVector::Zero());

	UFUNCTION()
	void DoorRock(UStaticMeshComponent* door);

protected:
	int Compass;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UBoxComponent>> BoxColliders;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMeshComponent>> Wall;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RoomCenter;



	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> ATypeData;
	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> BTypeData;
	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> CTypeData;
	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> DTypeData;

	

protected:

	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	bool MonsterAlive = true;
	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	bool Clear = false;
	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	bool FirstContact = false;

	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	float RoomSize;

	TArray<bool> RoomOn;
	TObjectPtr<UWorld> World;

	float LowZ = 926.0f;		// 기본값 : -74
	float HighZ = 1175.0f;	// 기본값 : 175
	float Alpha = 0.07f;	// 기본값 : 0.07
};

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

	UFUNCTION() // 룸에 처음 들어오면 발동하는 함수, 몬스터의 스폰 및 문을 잠그는 역할을 함
	void FirstBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() // 북쪽에 방을 자동으로 생성하는 함수
	bool SpawnNrothRoom(FVector INCenterLocation);
	UFUNCTION() // 남쪽에 방을 자동으로 생성하는 함수
	bool SpawnSouthRoom(FVector INCenterLocation);
	UFUNCTION() // 동쪽에 방을 자동으로 생성하는 함수
	bool SpawnEastRoom(FVector INCenterLocation);
	UFUNCTION() // 서쪽에 방을 자동으로 생성하는 함수
	bool SpawnWestRoom(FVector INCenterLocation);

	UFUNCTION() // 문이 자동으로 열리고 닫히게 하기 위한 함수
	void DoorUpDown(uint8 INSwitch, UStaticMeshComponent* INWallData);



protected:
	int Compass;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UBoxComponent>> BoxColliders; // 문의 충돌을 감지하는 변수들의 배열

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMeshComponent>> Wall; // 열리고 닫힐 문

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RoomCenter; // 룸의 중앙에서 몬스터를 스폰, 방을 생성하지 못하도록 하는 박스 콜리전



	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> ATypeData; // A타입의 룸 BP를 저장하기 위한 변수
	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> BTypeData; // B타입의 룸 BP를 저장하기 위한 변수
	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> CTypeData; // C타입의 룸 BP를 저장하기 위한 변수
	UPROPERTY(EditAnywhere, Category = "RoomDatas", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMRoomBase> DTypeData; // D타입의 룸 BP를 저장하기 위한 변수

	

protected:

	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	uint8 bMonsterAlive : 1;	// 몬스터의 생존을 파악하는 bool
	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	uint8 bClear : 1;			// 클리어를 기록하는 bool
	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	uint8 bFirstContact : 1;	// 룸의 첫 접촉을 기록하는 bool
	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	uint8 bDoorRock : 1;		// 모든 문을 걸어잠그기 위한 bool

	UPROPERTY(EditAnywhere, Category = "Switch", Meta = (AllowPrivateAccess = "true"))
	float RoomSize; // 룸의 사이즈를 저장하여 룸 스폰시 자동으로 스폰하기 위한 변수

	TObjectPtr<UWorld> World; // 월드의 포인터를 저장하기 위한 변수

	float LowZ = 926.0f;	// 기본값 : -74
	float HighZ = 1175.0f;	// 기본값 : 175
	float Alpha = 0.07f;	// 기본값 : 0.07
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Monster/MMMonsterSpawnType.h"
#include "Monster/MMMonsterSpawner.h"
#include "Monster/MMMonsterArea.h"
#include "Dungeon/MMRoomDesignBase.h"
#include "MMRoomBase.generated.h"

UCLASS()
class MYSTICMAZE_API AMMRoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMRoomBase();

	UFUNCTION() // 룸에 처음 들어오면 발동하는 함수, 몬스터의 스폰 및 잠긴 문을여는 역할을 함
	void RoomBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

	UFUNCTION() // 던전에서 몬스터들이 다 죽었을 경우 호출되는 함수
	void ClearSignal();

	void SetSpawner(class AMMMonsterSpawner* INData) { Spawner = INData; }

	SpawnType GetRandomEnumValue();
	

	// 몬스터 스포너 생성하기 위한 함수
	//void SpawnerSumon(FVector INLocation);

protected:
	//int Compass;

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

	// 방의 문 개수 이상의 숫자를 가질 필요가 없기 때문에 최대 15까지만 저장하도록 변수 선언
	uint8 CurDoorUp : 4;
	uint8 MaxDoorUp : 4;

	UPROPERTY(EditAnywhere, Category = "MonsterSpawnerData", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> MonsterSpawner;

	/*
	룸의 타입을 정하는 변수 : 몬스터를 스폰할 때 사용함
	0 : A 타입 네모
	1 : B 타입 직사각형
	2 : C 타입 ㄱ
	3 : D 타입 ㄴ
	*/
	uint8 RoomType : 2;

	// 몇 층에 스폰할지 정보를 전달하기 위한 함수
	// 0 : 필드
	// 1 : 1층
	// 2 : 2층
	// 3 : 3층
	UPROPERTY(EditAnywhere, Category = "MonsterSpawnerData", Meta = (AllowPrivateAccess = "true"))
	uint8 RoomFloor;
	
	UPROPERTY(EditAnywhere, Category = "MonsterSpawnerData", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AMMMonsterSpawner> Spawner;

	TObjectPtr<class AMMMonsterArea> MonsterArea;

	UPROPERTY(EditAnywhere, Category = "Design", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AMMRoomDesignBase>> Designs;
	void Structure_Installation(FVector INData);

};

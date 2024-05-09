// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dungeon/MMRoomBase.h"
#include "MMFristStartRoom.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMFristStartRoom : public AMMRoomBase
{
	GENERATED_BODY()
	
public:
	AMMFristStartRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void NorthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void NorthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void WastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void EastBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EastEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SouthBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SouthEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);





private: // 뷰포트에서 사용되는 변수들

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MainFloor; // 루트 컴포넌트가 되기 위한 메인 바닥

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> North_Collision; // 북쪽 문을 열고 닫고 방을 스폰하기 위한 콜리전
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Wast_Collision; // 서쪽 문을 열고 닫고 방을 스폰하기 위한 콜리전
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> East_Collision; // 동쪽 문을 열고 닫고 방을 스폰하기 위한 콜리전
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> South_Collision; // 남쪽 문을 열고 닫고 방을 스폰하기 위한 콜리전

private: // 룸 작동 변수

	// 문이 열리고 닫히는지를 판별하는 변수
	uint8 bNorth_Switch : 1;
	uint8 bSouth_Switch : 1;
	uint8 bWest_Switch : 1;
	uint8 bEast_Switch : 1;

	// 문 앞에 다른 룸이 있다면 룸의 생성을 막는 변수
	uint8 bNorth_Blocking : 1;
	uint8 bSouth_Blocking : 1;
	uint8 bWest_Blocking : 1;
	uint8 bEast_Blocking : 1;
};

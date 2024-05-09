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
	TObjectPtr<UStaticMeshComponent> MainFloor;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> North_Collision;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Wast_Collision;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> East_Collision;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> South_Collision;

private: // 룸 작동 변수

	bool North_Switch = false;
	bool South_Switch = false;
	bool West_Switch = false;
	bool East_Switch = false;

};

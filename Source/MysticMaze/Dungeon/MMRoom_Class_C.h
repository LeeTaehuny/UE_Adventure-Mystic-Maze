// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dungeon/MMRoomBase.h"
#include "MMRoom_Class_C.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMRoom_Class_C : public AMMRoomBase
{
	GENERATED_BODY()

public:
	AMMRoom_Class_C();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void NorthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void NorthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void NorthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void NorthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SouthBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SouthEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SouthBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SouthEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void WastBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WastEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void WastBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WastEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void EastBeginOverlap_0(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EastEndOverlap_0(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void EastBeginOverlap_1(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EastEndOverlap_1(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MainFloor1;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RoomCenter_1;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> North_0;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> South_0;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> East_0;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Weast_0;

	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> North_1;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> South_1;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> East_1;
	UPROPERTY(EditAnywhere, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Weast_1;

private: // 룸 작동 변수

	bool North_Switch_0 = false;
	bool North_Switch_1 = false;

	bool South_Switch_0 = false;
	bool South_Switch_1 = false;

	bool West_Switch_0 = false;
	bool West_Switch_1 = false;

	bool East_Switch_0 = false;
	bool East_Switch_1 = false;
};

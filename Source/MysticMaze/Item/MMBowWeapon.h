// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MMWeapon.h"
#include "MMBowWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMBowWeapon : public AMMWeapon
{
	GENERATED_BODY()
	
public:
	AMMBowWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	FORCEINLINE void SetIsHold(bool InValue) { bIsHold = InValue; }
	FORCEINLINE class AMMArrow* GetTempArrow() { return TempArrow; }
	virtual void EquipWeapon() override;

	void SpawnArrow();
	void SpawnArrow(TSubclassOf<AActor> InArrowClass, float InDamage);
	void ShootArrow();
	void DestroyArrow();

protected:
	FVector GetArrowSocketLocation(USkeletalMeshComponent* Mesh);

	UPROPERTY(EditAnywhere, Category = "Quiver", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> QuiverClass;

	UPROPERTY(VisibleAnywhere, Category = "Quiver", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ArrowClass;

	UPROPERTY(EditAnywhere, Category = "BaseSocketName", meta = (AllowPrivateAccess = "true"))
	FName StringSocketName;

	UPROPERTY()
	TObjectPtr<AActor> Quiver;

	UPROPERTY()
	TObjectPtr<class AMMArrow> TempArrow; 

	FName QuiverSocketName;
	FName ArrowSocketName;

private:
	uint8 bIsHold : 1;
	
	FVector BaseLocation;
	FVector StringLocation;
};

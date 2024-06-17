// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/MMCharacterStat.h"
#include "MMWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Sword,	// 검
	WT_Bow,		// 활
	WT_Staff,	// 지팡이
};

UCLASS()
class MYSTICMAZE_API AMMWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMMWeapon();

public:
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; }
	
public:
	virtual void EquipWeapon();
	void DrawWeapon(USkeletalMeshComponent* Mesh);
	void SheatheWeapon(USkeletalMeshComponent* Mesh);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPoseableMeshComponent> WeaponMesh;

	EWeaponType WeaponType;

	FMMCharacterStat WeaponStat;

	FName BaseSocketName;
	FName DrawSocketName;
};

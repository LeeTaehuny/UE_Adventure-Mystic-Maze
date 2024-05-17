// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MMCharacterStat.h"
#include "GameData/MMEnums.h"
#include "MMStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* CurrentHp */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementSpeedChangedDelegate, float /* MovementSpeed */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChangedDelegate, const FMMCharacterStat& /* Base Stat */, const FMMCharacterStat& /* Modifier Stat */, const FMMCharacterStat& /* Weapon Stat */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTICMAZE_API UMMStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMMStatComponent();

public:
	void SetLevel(int32 InLevel);
	void UpdateDetailStatus();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

protected:
	void InitPlayerStatus();
	void InitMonsterStatus(int32 InLevel);

	void SetHp(float NewHp);

// Delegate
public:
	FOnStatChangedDelegate OnStatChanged;
	FOnHpChangedDelegate OnHpChanged;
	FOnMovementSpeedChangedDelegate OnMovementSpeedChanged;

// Basic Status (STR, DEX, CON, INT)
protected:
	// 기본 스텟 (레벨별)
	UPROPERTY(Transient, VisibleAnywhere, Category = "Status", Meta = (AllowPrivateAccess = "true"))
	FMMCharacterStat BaseStat;

	// 자유 분배 스탯
	UPROPERTY(Transient, VisibleAnywhere, Category = "Status", Meta = (AllowPrivateAccess = "true"))
	FMMCharacterStat ModifierStat;

	// 장비 추가 스탯
	UPROPERTY(Transient, VisibleAnywhere, Category = "Status", Meta = (AllowPrivateAccess = "true"))
	FMMCharacterStat WeaponStat;

	// 최종 스탯
	UPROPERTY(Transient, VisibleAnywhere, Category = "Status", Meta = (AllowPrivateAccess = "true"))
	FMMCharacterStat TotalStat;

	// 잔여 스탯
	UPROPERTY(VisibleAnywhere, Category = "Status", Meta = (AllowPrivateAccess = "true"))
	int32 AvailableStatPoint;

// Current Status
protected:
	// 현재 레벨
	UPROPERTY(VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float CurrentLevel;

	// 최대 체력
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float MaxHp;

	// 현재 체력
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float CurrentHp;

	// 최대 마나
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float MaxMp;

	// 현재 마나
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float CurrentMp;

	// 공격력
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	// 방어력
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float Defense;

	// 이동속도
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	// 공격속도
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	// 치명타 확률
	UPROPERTY(Transient, VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float CriticalHitRate;

private:
	int32 CurrentExp;
	float MaxAdditiveMovementSpeed;
	float MaxAdditiveAttackSpeed;
	float MaxAdditiveCriticalHitRate;

	EClassType ClassType;
};

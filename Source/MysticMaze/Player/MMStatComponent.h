// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MMCharacterStat.h"
#include "GameData/MMEnums.h"
#include "MMStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /* CurrentHp */, float /* MaxHp */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMpChangedDelegate, float /* CurrentMp */, float /* MaxMp */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnExpChangedDelegate, float /* CurrentMp */, float /* MaxExp */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementSpeedChangedDelegate, float /* MovementSpeed */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, const FMMCharacterStat& /* Total Stat */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedDelegate, const FMMCharacterStat& /* Weapon Stat */);

UENUM(BlueprintType)
enum class EStatusType : uint8
{
	ST_STR, // 힘
	ST_DEX, // 민첩
	ST_CON, // 체력
	ST_INT, // 지력
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTICMAZE_API UMMStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMMStatComponent();

public:
	void Init();
	void SetLevel(int32 InLevel);
	void UpdateDetailStatus();

	// 데미지 적용
	float ApplyDamage(float InDamage);

	// 체력 회복
	void HealHp(float InHealPercent);
	// 마나 회복
	void HealMp(float InHealPercent);
	// 마나 사용
	void UseMp(float InAmount);

	// 스탯 업그레이드
	void UpgradeStat(EStatusType Type);

	// 장비 스탯 설정
	void SetWeaponStat(FMMCharacterStat InWeaponStat = FMMCharacterStat());

protected:
	virtual void BeginPlay() override;

protected:
	void InitPlayerStatus();
	void InitMonsterStatus(int32 InLevel);

	void SetHp(float NewHp);
	void SetMp(float NewMp);
	void SetExp(float NewExp);

// Delegate
public:
	FOnHpZeroDelegate OnHpZero;
	FOnStatChangedDelegate OnStatChanged;
	FOnWeaponChangedDelegate OnWeaponChanged;
	FOnHpChangedDelegate OnHpChanged;
	FOnMpChangedDelegate OnMpChanged;
	FOnMpChangedDelegate OnExpChanged;
	FOnMovementSpeedChangedDelegate OnMovementSpeedChanged;

// Getter
public:
	FORCEINLINE int32 GetCurrentLevel() { return CurrentLevel; }
	FORCEINLINE float GetMaxExp() { return MaxExp; }
	FORCEINLINE float GetCurrentExp() { return CurrentExp; }
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetMaxMp() { return MaxMp; }
	FORCEINLINE float GetCurrentMp() { return CurrentMp; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	FORCEINLINE float GetDefense() { return Defense; }
	FORCEINLINE float GetMovementSpeed() { return MovementSpeed; }
	FORCEINLINE float GetAttackSpeed() { return AttackSpeed; }
	FORCEINLINE float GetCriticalHitRate() { return CriticalHitRate; }
	FORCEINLINE int32 GetAvailableStatPoint() { return AvailableStatPoint; }
	FORCEINLINE FMMCharacterStat GetTotalStat() { return TotalStat; }
	FORCEINLINE FMMCharacterStat GetWeaponStat() { return WeaponStat; }
	FORCEINLINE EClassType GetClass() { return ClassType; }

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
	int32 CurrentLevel;

	// 최대 경험치
	UPROPERTY(VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float MaxExp;

	// 현재 경험치
	UPROPERTY(VisibleAnywhere, Category = "DetailStatus", Meta = (AllowPrivateAccess = "true"))
	float CurrentExp;

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
	float MaxAdditiveMovementSpeed;
	float MaxAdditiveAttackSpeed;
	float MaxAdditiveCriticalHitRate;

	EClassType ClassType;
};

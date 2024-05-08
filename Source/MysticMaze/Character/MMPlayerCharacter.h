﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MMCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/MMAnimationAttackInterface.h"
#include "Interface/MMAnimationUpdateInterface.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "GameData/MMEnums.h"
#include "MMPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMPlayerCharacter : public AMMCharacterBase, public IMMAnimationAttackInterface, public IMMAnimationUpdateInterface, public IMMAnimationWeaponInterface, public IMMPlayerVisualInterface
{
	GENERATED_BODY()
	
public:
	AMMPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Camera Section
protected:
	FORCEINLINE virtual class UCameraComponent* GetPlayerCamera() override { return Camera; }

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

// Input Section
protected:
	// 공용
	void DashStart();
	void DashEnd();
	void RollStart();
	void RollEnd(class UAnimMontage* Montage, bool IsEnded);

	// Basic
	void BasicMove(const FInputActionValue& Value);
	void BasicLook(const FInputActionValue& Value);
	void BasicAttack();

	// Warrior
	void GuardStart();
	void GuardEnd();

	// Archer
	void DrawArrow();
	void ReleaseArrow();

	// 공용
	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Dash;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Roll;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertWeapon;

	// InputMappingContext
	TMap<EClassType, TObjectPtr<class UInputMappingContext>> IMC_Array;

	// Basic Input
	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_BasicMove;

	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_BasicLook;

	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_BasicAttack;

	// Input Warrior
	UPROPERTY(VisibleAnywhere, Category = ClassInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_WarriorGuard;

	// Input Archer
	UPROPERTY(VisibleAnywhere, Category = ClassInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ArcherDraw;

	// Input Mage
	UPROPERTY(VisibleAnywhere, Category = ClassInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_MageSave;

// Montage
protected:
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TMap<EClassType, TObjectPtr<class UAnimMontage>> ComboMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TMap<EClassType, TObjectPtr<class UAnimMontage>> DrawMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TMap<EClassType, TObjectPtr<class UAnimMontage>> SheatheMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DrawArrowMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ReleaseArrowMontage;

// ComboData
protected:
	UPROPERTY(EditAnywhere, Category = ComboData, Meta = (AllowPrivateAccess = "true"))
	TMap<EClassType, TObjectPtr<class UMMComboActionData>> ComboData;

// Combo Section
protected:
	void ComboStart();
	void ComboEnd(class UAnimMontage* Montage, bool IsEnded);
	void ComboCheck();
	void SetComboTimer();

	// 콤보에 사용될 타이머 변수
	FTimerHandle ComboTimerHandle;
	// 현재 콤보 진행 수
	int32 CurrentComboCount;
	// 콤보 입력 판별
	uint8 bHasComboInput : 1;

// Attack Section
protected:
	virtual void BaseAttackCheck() override;

// Character Class Section
protected:
	FORCEINLINE virtual EClassType GetClassType() override { return ClassType; };

	void ChangeClass(EClassType Class);

	EClassType ClassType;

// Weapon Section
protected:
	FORCEINLINE virtual class AMMWeapon* GetWeapon() override { return CurrentWeapon; }
	void ConvertWeapon();
	void DrawWeapon();
	void DrawEnd(class UAnimMontage* Montage, bool IsEnded);
	void SheatheWeapon();
	void SheatheEnd(class UAnimMontage* Montage, bool IsEnded);
	void EquipWeapon(class AMMWeapon* Weapon);

	// TEST
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AMMWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AMMWeapon> CurrentWeapon;

// Archer Section
protected:
	void DrawArrowEnd(class UAnimMontage* Montage, bool IsEnded);
	void ReleaseArrowEnd(class UAnimMontage* Montage, bool IsEnded);
	void ShootArrow();

// Member Variable
protected:
	FORCEINLINE virtual bool GetIsGuard() override { return bIsGuard; }
	FORCEINLINE virtual bool GetIsEquip() override { return bIsEquip; }
	FORCEINLINE virtual bool GetIsHold() override { return bIsHold; }

	uint8 bIsChange : 1;		// 무기 교체
	uint8 bIsDash : 1;			// 달리기 여부
	uint8 bIsRoll : 1;			// 구르기 여부
	uint8 bIsAttacking : 1;		// 공격중인지 체크
	uint8 bIsGuard : 1;			// 방어 여부
	uint8 bIsHold : 1;			// 활 Draw 여부
	uint8 bCanShoot : 1;		// 화살 발사 가능 여부
	uint8 bIsStop : 1;			// 애니메이션 스탑 여부
	uint8 bIsEquip : 1;			// 무기 장착 여부

	float WalkSpeed;
	float RunSpeed;
};
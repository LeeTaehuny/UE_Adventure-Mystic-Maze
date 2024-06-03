// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MMCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/MMAnimationAttackInterface.h"
#include "Interface/MMAnimationUpdateInterface.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Interface/MMPlayerClassInterface.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "Interface/MMInventoryInterface.h"
#include "Interface/MMStatusInterface.h"
#include "Interface/MMSkillInterface.h"
#include "MMPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMPlayerCharacter : public AMMCharacterBase, public IMMAnimationAttackInterface, public IMMAnimationUpdateInterface, public IMMAnimationWeaponInterface, public IMMPlayerVisualInterface, public IMMInventoryInterface,
	public IMMPlayerClassInterface, public IMMStatusInterface, public IMMSkillInterface
{
	GENERATED_BODY()
	
public:
	AMMPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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
	void ConvertInventoryVisibility();
	void ConvertStatusVisibility();
	void ConvertEquipmentVisibility();
	void ConvertSkillVisibility();
	void ConvertRiding();
	void Interaction();
	void UseQuickSlot(int32 InNum);

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

	// Mage
	void SaveStart();
	void SaveEnd();

	// 공용
	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Dash;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Roll;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertWeapon;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Interaction;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertInventory;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertStatus;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertEquipment;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertSkill;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_ConvertRiding;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_QuickSlot1;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_QuickSlot2;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_QuickSlot3;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_QuickSlot4;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_QuickSlot5;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_QuickSlot6;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC_Riding;

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
	FORCEINLINE virtual class UAnimMontage* GetPickUpMontage() override { return PickUpMontage; }

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> PickUpMontage;

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
	void Hit();
	void Death();
	void DeathEnd(class UAnimMontage* Montage, bool IsEnded);
	void Respawn();

// Character Class Section
protected:
	FORCEINLINE virtual EClassType GetClassType() override { return ClassType; };
	FORCEINLINE virtual EClassType GetClass() override { return ClassType; }
	FORCEINLINE virtual void SetClass(EClassType Class) override { ClassType = Class; ChangeClass(ClassType); }
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
	virtual void EquipWeapon(class AMMWeapon* Weapon) override;
	virtual void UnEquipWeapon() override;

	UPROPERTY(VisibleAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AMMWeapon> CurrentWeapon;

// Warrior Section
protected:
	FORCEINLINE virtual bool GetIsGuard() override { return bIsGuard; }

	uint8 bIsGuard : 1;			// 방어 여부

// Archer Section
protected:
	FORCEINLINE virtual bool GetIsHold() override { return bIsHold; }

	uint8 bIsHold : 1;			// 활 Draw 여부
	uint8 bCanShoot : 1;		// 화살 발사 가능 여부

	void DrawArrowEnd(class UAnimMontage* Montage, bool IsEnded);
	void ReleaseArrowEnd(class UAnimMontage* Montage, bool IsEnded);
	void ShootArrow();

// Mage Section
protected:
	FORCEINLINE virtual bool GetIsCharge() override { return bIsCharge; }

	uint8 bIsCharge : 1;		// 마나 축적 여부
	float SumTime;

	UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> ChargeParticle;

// Inventory Section
protected:
	FORCEINLINE virtual class UMMInventoryComponent* GetInventoryComponent() override { return Inventory; }

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMMInventoryComponent> Inventory;

// Skill Section
protected:
	FORCEINLINE virtual class UMMSkillComponent* GetSkillComponent() override { return Skill; }

	UPROPERTY(VisibleAnywhere, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMMSkillComponent> Skill;

// Stat Section
protected:
	FORCEINLINE virtual class UMMStatComponent* GetStatComponent() override { return Stat; }

	void ApplyMovementSpeed(float MovementSpeed);

// Riding Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "Ride", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RideMesh;

	UPROPERTY(VisibleAnywhere, Category = "Ride", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RideActorMesh;

// Member Variable
protected:
	FORCEINLINE virtual bool GetIsEquip() override { return bIsEquip; }

	uint8 bIsChange : 1;		// 무기 교체
	uint8 bIsDash : 1;			// 달리기 여부
	uint8 bIsRoll : 1;			// 구르기 여부
	uint8 bIsAttacking : 1;		// 공격중인지 체크
	uint8 bIsStop : 1;			// 애니메이션 스탑 여부
	uint8 bIsEquip : 1;			// 무기 장착 여부
	uint8 bIsRide : 1;			// 탈것 활성화 여부

	float WalkSpeed;
	float RunSpeed;
	float RidingSpeed;

	// 플레이어 입력 방향
	FVector2D MovementVector;

// Particle Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> ChargeParticleSystemComponent;
};

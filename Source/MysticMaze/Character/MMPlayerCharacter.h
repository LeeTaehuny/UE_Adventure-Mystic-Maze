// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MMCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/MMAnimationAttackInterface.h"
#include "MMPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMPlayerCharacter : public AMMCharacterBase, public IMMAnimationAttackInterface
{
	GENERATED_BODY()
	
public:
	AMMPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Camera Section
protected:
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

	// 공용
	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Dash;

	UPROPERTY(VisibleAnywhere, Category = CommonInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Roll;

	// Basic Input
	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC_Basic;

	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_BasicMove;

	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_BasicLook;

	UPROPERTY(VisibleAnywhere, Category = BaseInput, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_BasicAttack;

// Montage
protected:
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

	// TODO : 무기가 추가된 이후 여러 종류의 공격이 존재할 경우 TMap으로 관리하기(현재 상태 - 콤보 몽타주)
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BasicComboMontage;

// ComboData
protected:
	UPROPERTY(EditAnywhere, Category = ComboData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMMComboActionData> BasicComboData;

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

// Member Variable
protected:
	uint8 bIsDash : 1;
	uint8 bIsRoll : 1;
	uint8 bIsAttacking : 1;

	float WalkSpeed;
	float RunSpeed;
};

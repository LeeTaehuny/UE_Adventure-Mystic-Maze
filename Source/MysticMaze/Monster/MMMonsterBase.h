// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MMCharacterBase.h"
#include "Components/SphereComponent.h"

#include "Interface/MMMonsterDieInterface.h"
#include "Interface/MMMonsterATKChekInterface.h"
#include "Interface/MMMonsterATKModeInterface.h"
#include "Interface/MMWidgetInterface.h"
#include "Kismet/GameplayStatics.h"

#include "Player/MMStatComponent.h"

#include "MMMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMMonsterBase : public AMMCharacterBase, public IMMWidgetInterface
{
	GENERATED_BODY()

public:
	AMMMonsterBase();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PostInitializeComponents() override;

	virtual void MonsterDieMontage();

	UFUNCTION()
	void ATKBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ATKEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetCenterLocation(FVector InLocation);
	FORCEINLINE void SetATKMode(bool INData) { ATK_Mode = INData; }
	FORCEINLINE bool GetATKMode() { return ATK_Mode; }

	void SetStatLevel(int32 INData);

	void ApplyMovementSpeed(float MovementSpeed);
	void Die();
	void Hit();

	// IMMWidgetInterface : 몬스터 위젯을 초기화하기 위한 인터페이스 함수
	virtual void SetupCharacterWidget(class UMMCustomWidget* InUserWidget) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bDie : 1;

	UPROPERTY(EditAnywhere, Category = "ATK", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> ATK_Collision;

	UPROPERTY(EditAnywhere, Category = "ATK", Meta = (AllowPrivateAccess = "true"))
	uint8 ATK_Mode : 1;

	// 스폰지점을 저장하기 위한 변수
	FVector CenterLocation;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DieMontage;

	UPROPERTY(EditAnywhere, Category = "Damage Text", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AMM_MonsterDamageText> Monster_Damage;

	UPROPERTY(EditAnywhere, Category = "Damage Text", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> LocationData;

	float HP_Percent = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMMWidgetComponent> HpBar;
	
};

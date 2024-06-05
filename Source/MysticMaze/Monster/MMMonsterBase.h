// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MMCharacterBase.h"
#include "Components/SphereComponent.h"

#include "Interface/MMMonsterDieInterface.h"
#include "Interface/MMMonsterATKChekInterface.h"
#include "Interface/MMMonsterATKModeInterface.h"
#include "Kismet/GameplayStatics.h"

#include "Player/MMStatComponent.h"

#include "MMMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMMonsterBase : public AMMCharacterBase
{
	GENERATED_BODY()

public:
	AMMMonsterBase();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PostInitializeComponents() override;

	virtual void GoblindDieMontage();

	UFUNCTION()
	void ATKBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ATKEndOverlap(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetCenterLocation(FVector InLocation);
	FORCEINLINE void SetATKMode(bool INData) { ATK_Mode = INData; }
	FORCEINLINE bool GetATKMode() { return ATK_Mode; }


	void ApplyMovementSpeed(float MovementSpeed);
	void Die();
	void Hit();

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
	TObjectPtr<class UAnimMontage> Die1Montage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Die2Montage;
	
};

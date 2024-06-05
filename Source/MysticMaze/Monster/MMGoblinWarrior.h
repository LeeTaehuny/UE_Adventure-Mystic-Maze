// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMAnimationGobWarriorInterface.h"	// 애니메이션 블루프린트에 값을 전달하기 위한 인터페이스
#include "Interface/MMGoblinWarriorAIDataInterface.h"	// ai 컨트롤러에 값을 전달하기 위한 인터페이스
#include "Interface/MMGoblinHowlAnimationInterface.h"	// 고블린이 포효 애니메이션을 재생하기 위한 인터페이스
#include "Interface/MMWarriorATKMontageInterface.h"		// 고블린 전사의 공격 애니메이션을 실행시키기 위한 인터페이스

#include "MMGoblinWarrior.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGoblinWarrior : public AMMMonsterBase,
	public IMMMonsterATKChekInterface,
	public IMMMonsterATKModeInterface,
	public IMMAnimationGobWarriorInterface,
	public IMMGoblinWarriorAIDataInterface,
	public IMMGoblinHowlAnimationInterface,
	public IMMWarriorATKMontageInterface,
	public IMMMonsterDieInterface
{
	GENERATED_BODY()
	

public:
	AMMGoblinWarrior();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GoblindDieMontage() override;

protected:
	void MontageAutoPlay(UAnimMontage* INBodyData, UAnimMontage* INWeaponData);

	// IMMMonsterATKChekInterface : 공격을 감지하는 인터페이스
	virtual void ATKChecking() override;

	// IMMMonsterATKModeInterface : 플레이어를 찾았을 경우 사용되는 함수
	FORCEINLINE virtual void SetATKMode(bool INData) override { ATK_Mode = INData; }
	FORCEINLINE virtual bool GetATKMode() override { return ATK_Mode; }

	// IMMAnimationGobWarriorInterface : 애니메이션 블루프린트에 값을 전달하기 위한 인터페이스
	FORCEINLINE virtual bool GetDie() override { return bDie; }
	FORCEINLINE virtual bool GetDieMotion() override { return bRandomDieMotion; }

	// IMMGoblinHowlAnimationInterface : 고블린이 포효 애니메이션을 재생하기 위한 인터페이스
	virtual void PlayingHowl() override;

	// IMMWarriorATKMontageInterface : 고블린 전사의 공격 애니메이션을 실행시키기 위한 인터페이스
	virtual void WarriorNormalATK() override;
	FORCEINLINE virtual bool PlayingAnimation() override { return GetMesh()->GetAnimInstance()->Montage_IsPlaying(BodyATK); }

	// IMMMonsterDieInterface : 몬스터가 죽을때 실행되는 함수
	virtual void Monsterdie() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadMesh;			// 헬멧 방어구
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BodyArmorMesh;		// 몸통 방어구
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BracersMesh;			// 손목 방어구
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkirtMesh;			// 치마..?
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LegArmorMesh;		// 다리 방어구

	UPROPERTY(EditAnywhere, Category = "Goblin Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SwordMesh;			// 검
	UPROPERTY(EditAnywhere, Category = "Goblin Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ShieldMesh;			// 방패


	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BodyHowl;
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> WeaponHowl;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BodyATK;
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> WeaponATK;

protected:

	uint8 bRandomDieMotion : 1;

};

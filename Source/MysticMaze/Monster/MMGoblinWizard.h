// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MMMonsterBase.h"

#include "Interface/MMAnimationGobWizardInterface.h"			// 애니메이션 블루프린트에 값을 전달하기 위한 인터페이스
#include "Interface/MMGoblinWizardAIDataInterface.h"			// ai 컨트롤러에 값을 전달하기 위한 인터페이스
#include "Interface/MMGoblinHowlAnimationInterface.h"			// 고블린이 포효 애니메이션을 재생하기 위한 인터페이스
#include "Interface/MMWizardNormalATKInterface.h"				// 고블린이 공격을 하기 위한 인터페이스
#include "Interface/MMWizardATKMontageInterface.h"				// 마법사가 공격 모션을 재생하기 위한 인터페이스
#include "Interface/MMWizardDealerServiceInterface.h"			// 마법사의 딜러 서비스 노드에 값을 전달하기 위한 인터페이스
#include "Interface/MMStatusInterface.h"						// 스텟 컴포넌트를 반환하기 위한 인터페이스

#include "Monster/Magic/MMFireBall.h"

#include "MMGoblinWizard.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMGoblinWizard : public AMMMonsterBase,
	public IMMMonsterATKChekInterface,
	public IMMMonsterATKModeInterface,
	public IMMAnimationGobWizardInterface,
	public IMMGoblinWizardAIDataInterface,
	public IMMGoblinHowlAnimationInterface,
	public IMMMonsterDieInterface,
	public IMMWizardNormalATKInterface,
	public IMMWizardATKMontageInterface,
	public IMMWizardDealerServiceInterface,
	public IMMStatusInterface
{
	GENERATED_BODY()


public:
	AMMGoblinWizard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MonsterDieMontage() override;

protected:
	void MontageAutoPlay(UAnimMontage* INBodyData, UAnimMontage* INWeaponData);
	void MontageAutoSection(UAnimMontage* INBodyData, UAnimMontage* INWeaponData, FName INData);

	// IMMMonsterATKChekInterface : 공격을 감지하는 인터페이스
	virtual void ATKChecking() override;

	// IMMMonsterATKModeInterface : 플레이어를 찾았을 경우 사용되는 함수
	FORCEINLINE virtual void SetATKMode(bool INData) override { ATK_Mode = INData; }
	FORCEINLINE virtual bool GetATKMode() override { return ATK_Mode; }

	// IMMAnimationGobWizardInterface : 애니메이션 블루프린트에 값을 전달하기 위한 인터페이스
	FORCEINLINE virtual bool GetDie() override { return bDie; }
	FORCEINLINE virtual bool GetDieMotion() override { return bRandomDieMotion; }

	// IMMGoblinHowlAnimationInterface : 고블린이 포효 애니메이션을 재생하기 위한 인터페이스
	virtual void PlayingHowl() override;

	// IMMMonsterDieInterface : 몬스터가 죽을때 실행되는 함수
	virtual void Monsterdie() override;

	// IMMWizardNormalATKInterface : 공격용
	virtual void StartATK() override;

	// IMMWizardATKMontageInterface : 공격 몽타주 재생용
	virtual void StartATKMonatage(int INData) override;
	virtual bool GetMontagePlaying() override;

	// IMMWizardDealerServiceInterface : 마법사의 딜러 서비스 노드에 값을 전달하기 위한 인터페이스
	FORCEINLINE virtual float GetMInSecondATK() override { return NormalAtK_MinSecond; }
	FORCEINLINE virtual float GetMaxSecondATK() override { return NormalAtK_MaxSecond; }
	FORCEINLINE virtual void SetMInSecondATK(float INData) override { NormalAtK_MinSecond = INData; };
	FORCEINLINE virtual bool GetNormalATK() override { return NormalATKSign; }
	FORCEINLINE virtual void SetNormalATK(bool INData) override { NormalATKSign = INData; }
	FORCEINLINE virtual void StopMove() override;

	// IMMStatusInterface : 마법사의 스텟 컴포넌트를 반환하기 위한 인터페이스
	FORCEINLINE virtual class UMMStatComponent* GetStatComponent() override { return Stat; }

	virtual void MonsterHitAnim() override;
protected:

	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadMesh;			// 헬멧 방어구
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BodyArmorMesh;		// 몸통 방어구
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BracersMesh;			// 손목 방어구
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> PantsMesh;			// 바지
	UPROPERTY(EditAnywhere, Category = "Goblin Armor", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BottleMesh;			// 다리 방어구

	UPROPERTY(EditAnywhere, Category = "Goblin Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> StaffMesh;			// 스테프



	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BodyHowl;
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> WeaponHowl;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BodyNormalATK;
	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> WeaponNormalATK;


	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Die2Montage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> Hit2Montage;

protected:
	uint8 bRandomDieMotion : 1;

protected:

	UPROPERTY(EditAnywhere, Category = "Magic", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMMMonsterMagicBase> FireBallBPData;

	//TObjectPtr<AMMMonsterMagicBase> FireVall;

	float NormalAtK_MinSecond = 0.0f;
	float NormalAtK_MaxSecond = 7.0f;

	uint8 NormalATKSign : 1;

	float DieTimer = 0.0f;
};

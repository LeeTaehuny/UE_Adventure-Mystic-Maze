// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMStatComponent.h"
#include "Interface/MMPlayerClassInterface.h"
#include "Game/MMGameInstance.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMMStatComponent::UMMStatComponent()
{
	bWantsInitializeComponent = true;

	CurrentLevel = 1;
	AvailableStatPoint = 0;

	MaxAdditiveMovementSpeed = 400.0f;
	MaxAdditiveAttackSpeed = 1.5f;
	MaxAdditiveCriticalHitRate = 100.0f;
}

void UMMStatComponent::Init()
{
	IMMPlayerClassInterface* PlayerCharacter = Cast<IMMPlayerClassInterface>(GetOwner());
	if (PlayerCharacter)
	{
		// 플레이어 데이터 초기화
		InitPlayerStatus();

		// 세부 스탯 업데이트
		UpdateDetailStatus();

		// 현재 체력 초기화
		SetHp(MaxHp);
		SetMp(MaxMp);
		SetExp(CurrentExp);
	}
}

// Called when the game starts
void UMMStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMMStatComponent::InitPlayerStatus()
{
	// TODO : 세이브 파일로부터 데이터 읽어오기 (레벨, 초기 스탯 정보(ModifierStat))
	{
		// 레벨 및 스탯 포인트 초기화
		CurrentLevel = 1;
		AvailableStatPoint = 5;

		// ModifierStat 초기화
		FMMCharacterStat LoadModifierStatus;
		LoadModifierStatus.STR = 5;
		LoadModifierStatus.DEX = 5;
		LoadModifierStatus.CON = 5;
		LoadModifierStatus.INT = 5;

		ModifierStat = LoadModifierStatus;

		// WeaponStat 초기화 (무기가 있는 경우라면 나중에 인벤쪽에서 해주자..!)
		//FMMCharacterStat LoadWeaponStatus;
		//
		//LoadWeaponStatus.STR = 10;
		//LoadWeaponStatus.DEX = 10;
		//LoadWeaponStatus.CON = 10;
		//LoadWeaponStatus.INT = 10;
		//
		//WeaponStat = LoadWeaponStatus;

		// 클래스 정보 초기화
		ClassType = EClassType::CT_Warrior;

		// 현재 경험치 초기화
		CurrentExp = 10.0f;
	}
	
	// 레벨별 기본 스탯 적용하기
	UMMGameInstance* GameMode = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameMode)
	{
		BaseStat = GameMode->GetPlayerStat(CurrentLevel);
		MaxExp = BaseStat.EXP;
	}

	// 플레이어 직업 설정
	IMMPlayerClassInterface* PlayerCharacter = Cast<IMMPlayerClassInterface>(GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->SetClass(ClassType);
	}
}

void UMMStatComponent::InitMonsterStatus(int32 InLevel)
{
	// 레벨별 기본 스탯 적용하기
	UMMGameInstance* GameMode = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameMode)
	{
		BaseStat = GameMode->GetPlayerStat(CurrentLevel);
	}
}

void UMMStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	// 변경 이벤트 발생
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UMMStatComponent::SetMp(float NewMp)
{
	CurrentMp = FMath::Clamp<float>(NewMp, 0.0f, MaxMp);

	// 변경 이벤트 발생
	OnMpChanged.Broadcast(CurrentMp, MaxMp);
}

void UMMStatComponent::SetExp(float NewExp)
{
	CurrentExp = FMath::Clamp<float>(NewExp, 0.0f, MaxExp);

	// 변경 이벤트 발생
	OnExpChanged.Broadcast(CurrentExp, MaxExp);
}

void UMMStatComponent::UpdateDetailStatus()
{
	// Total Stat 업데이트
	TotalStat = BaseStat + ModifierStat + WeaponStat;

	// 비율에 맞춰 세부 스탯 업데이트
	// * 최대 체력
	MaxHp = (TotalStat.STR * 0.5f * 100) + (TotalStat.CON * 0.5f * 100);
	if (CurrentHp > MaxHp)
		SetHp(MaxHp);

	// * 최대 마나
	MaxMp = TotalStat.INT * 100;
	if (CurrentMp > MaxMp)
		SetMp(MaxMp);

	// * 공격력
	switch (ClassType)
	{
	case EClassType::CT_None:
	case EClassType::CT_Beginner:
	case EClassType::CT_Warrior :
		AttackDamage = TotalStat.STR * 10;
		break;

	case EClassType::CT_Archer:
		AttackDamage = TotalStat.DEX * 10;
		break;

	case EClassType::CT_Mage:
		AttackDamage = TotalStat.INT * 10;
		break;
	}
	// * 방어력
	Defense = TotalStat.CON * 5;
	// * 이동속도
	MovementSpeed = MaxAdditiveMovementSpeed * (TotalStat.DEX / 250.0f) + 600.0f;
	// * 공격속도
	AttackSpeed = MaxAdditiveAttackSpeed * (TotalStat.DEX / 250.0f) + 1.0f;
	// * 치명타 확률
	CriticalHitRate = MaxAdditiveCriticalHitRate * (TotalStat.DEX / 250.0f);

	// 이벤트 발생
	OnMovementSpeedChanged.Broadcast(MovementSpeed);
	OnStatChanged.Broadcast(TotalStat);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	OnMpChanged.Broadcast(CurrentMp, MaxMp);
}

float UMMStatComponent::ApplyDamage(float InDamage)
{
	// 데미지 연산
	const float PrevHp = CurrentHp;
	// * 실제 데미지 = (받은 데미지 - 방어력)
	const float ActualDamage = FMath::Clamp<float>(InDamage - Defense, 0, InDamage);

	// 데미지 적용
	SetHp(PrevHp - ActualDamage);

	// 현재 체력이 0보다 작은지 비교
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// 사망 이벤트 발생
		OnHpZero.Broadcast();
	}

	// 실제 받은 데미지 반환
	return ActualDamage;
}

void UMMStatComponent::HealHp(float InHealPercent)
{
	// 회복할 체력량 구해줍니다.
	float HealAmount = MaxHp * (InHealPercent / 100);

	// 체력을 더해 줍니다.
	SetHp(CurrentHp + HealAmount);
}

void UMMStatComponent::HealMp(float InHealPercent)
{
	// 회복할 마나량 구해줍니다.
	float HealAmount = MaxMp * (InHealPercent / 100);

	// 마나를 더해줍니다.
	SetMp(CurrentMp + HealAmount);
}

void UMMStatComponent::UseMp(float InAmount)
{
	SetMp(CurrentMp - InAmount);
}

void UMMStatComponent::UpgradeStat(EStatusType Type)
{
	// 잔여 포인트가 존재하는 경우
	if (AvailableStatPoint > 0)
	{
		// 타입에 따라 스탯을 강화합니다.
		switch (Type)
		{
		case EStatusType::ST_STR:
			ModifierStat.STR++;
			break;

		case EStatusType::ST_DEX:
			ModifierStat.DEX++;
			break;

		case EStatusType::ST_CON:
			ModifierStat.CON++;
			break;

		case EStatusType::ST_INT:
			ModifierStat.INT++;
			break;
		}

		// 잔여 포인트를 차감합니다.
		AvailableStatPoint--;

		// 스탯을 적용합니다.
		UpdateDetailStatus();
	}
}

void UMMStatComponent::SetWeaponStat(FMMCharacterStat InWeaponStat)
{
	WeaponStat = InWeaponStat;

	// 스탯을 업데이트 합니다.
	UpdateDetailStatus();

	OnWeaponChanged.Broadcast(WeaponStat);
}

void UMMStatComponent::SetLevel(int32 InLevel)
{
	CurrentLevel = InLevel;

	ClassType = EClassType::CT_None;
	InitMonsterStatus(CurrentLevel);
}


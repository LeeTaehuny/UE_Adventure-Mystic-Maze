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

void UMMStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	IMMPlayerClassInterface* PlayerCharacter = Cast<IMMPlayerClassInterface>(GetOwner());
	if (PlayerCharacter)
	{
		InitPlayerStatus();
	}
}

// Called when the game starts
void UMMStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 세부 스탯 업데이트
	UpdateDetailStatus();

	// 현재 체력 초기화
	SetHp(MaxHp);
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

		// WeaponStat 초기화
		FMMCharacterStat LoadWeaponStatus;

		LoadWeaponStatus.STR = 10;
		LoadWeaponStatus.DEX = 10;
		LoadWeaponStatus.CON = 10;
		LoadWeaponStatus.INT = 10;

		WeaponStat = LoadWeaponStatus;

		// 클래스 정보 및 경험치 초기화
		ClassType = EClassType::CT_Beginner;
		CurrentExp = 10;
	}
	
	// 레벨별 기본 스탯 적용하기
	UMMGameInstance* GameMode = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameMode)
	{
		BaseStat = GameMode->GetPlayerStat(CurrentLevel);
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
	OnHpChanged.Broadcast(CurrentHp);
}

void UMMStatComponent::UpdateDetailStatus()
{
	// Total Stat 업데이트
	TotalStat = BaseStat + ModifierStat + WeaponStat;

	// 비율에 맞춰 세부 스탯 업데이트
	// * 최대 체력
	MaxHp = (TotalStat.STR * 0.5f * 100) + (TotalStat.CON * 0.5f * 100);
	// * 최대 마나
	MaxMp = TotalStat.INT * 100;
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
	OnStatChanged.Broadcast(BaseStat, ModifierStat, WeaponStat);
}


void UMMStatComponent::SetLevel(int32 InLevel)
{
	CurrentLevel = InLevel;

	ClassType = EClassType::CT_None;
	InitMonsterStatus(CurrentLevel);
}


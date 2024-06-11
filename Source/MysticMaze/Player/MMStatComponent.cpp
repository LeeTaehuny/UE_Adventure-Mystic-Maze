// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMStatComponent.h"
#include "Interface/MMPlayerClassInterface.h"
#include "Game/MMGameInstance.h"
#include "Game/MMSaveGameData.h"

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
	AdditiveStatPoint = 3;
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
	}
}

// Called when the game starts
void UMMStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMMStatComponent::InitPlayerStatus()
{
	// GameInstance에서 Save파일 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	UMMSaveGameData* GameData = GameInstance->GetSaveData();
	// 저장된 게임 데이터가 있는 경우라면?
	if (GameData)
	{
		// 레벨 및 스탯 포인트 초기화
		CurrentLevel = GameData->Level;
		AvailableStatPoint = GameData->StatPoint;

		// ModifierStat 초기화
		ModifierStat = GameData->ModifierStatus;

		// 클래스 정보 초기화
		EClassType Type = GameData->Class;

		SetClass(Type);

		// 현재 경험치 초기화
		CurrentExp = GameData->CurrentExp;
	}
	
	// 레벨별 기본 스탯 적용하기
	UMMGameInstance* GameMode = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameMode)
	{
		BaseStat = GameMode->GetPlayerStat(CurrentLevel);
		MaxExp = BaseStat.EXP;
	}

	OnClassChanged.Broadcast();
}

void UMMStatComponent::InitMonsterStatus(int32 InLevel)
{
	// 레벨별 기본 스탯 적용하기
	UMMGameInstance* GameMode = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameMode)
	{
		BaseStat = GameMode->GetMonsterStat(CurrentLevel);

		UpdateDetailStatus();

		// 현재 체력 초기화
		SetHp(MaxHp);
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
	if (CurrentLevel == 50) return;
	CurrentExp += NewExp;

	if (MaxExp && CurrentExp >= MaxExp)
	{
		if (CurrentLevel == 49)
		{
			CurrentLevel++;
			CurrentExp = MaxExp;
			// 변경 이벤트 발생
			OnLevelUp.Broadcast();
			OnExpChanged.Broadcast(CurrentExp, MaxExp);
			UpdateDetailStatus();
			return;
		}

		while (CurrentExp >= MaxExp)
		{
			UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (!GameInstance) return;
			
			// 레벨 증가
			CurrentLevel++;
			// 스탯 포인트 증가
			AvailableStatPoint += AdditiveStatPoint;
			// 현재 경험치 감소
			CurrentExp -= MaxExp;
			// 최대 스탯 레벨에 맞춰 지정
			BaseStat = GameInstance->GetPlayerStat(CurrentLevel);
			// 최대 경험치 초기화
			MaxExp = BaseStat.EXP;
		}
		// TODO : 이펙트 or 사운드 추가하기

		// 세부 스탯 업데이트
		UpdateDetailStatus();

		// 레벨업 이벤트 발생 (전직 교관 위젯에서 사용할 예정)
		OnLevelUp.Broadcast();
	}

	// 변경 이벤트 발생
	OnExpChanged.Broadcast(CurrentExp, MaxExp);
}

void UMMStatComponent::SaveStat()
{
	// GameInstance에서 Save파일 이름 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	// 게임 데이터 인스턴스 받아오기
	UMMSaveGameData* GameData = GameInstance->GetSaveData();
	if (GameData)
	{
		// 직업 저장
		GameData->Class = ClassType;

		// 레벨 및 경험치 저장
		GameData->Level = CurrentLevel;
		GameData->CurrentExp = CurrentExp;

		// 스탯 잔여 포인트 저장
		GameData->StatPoint = AvailableStatPoint;

		// 분배된 스탯 저장
		GameData->ModifierStatus = ModifierStat;

		// 게임 저장하기
		UGameplayStatics::SaveGameToSlot(GameData, GameData->SaveSlotName, GameData->SaveIndex);
	}
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
	if (CurrentHp == KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

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
	else
	{
		// 피격 이벤트 발생
		OnHit.Broadcast();
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

void UMMStatComponent::SetClass(EClassType Type)
{
	if (ClassType == Type) return;

	ClassType = Type;

	IMMPlayerClassInterface* PlayerCharacter = Cast<IMMPlayerClassInterface>(GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->SetClass(ClassType);
	}

	OnClassChanged.Broadcast();
}

void UMMStatComponent::SetLevel(int32 InLevel)
{
	CurrentLevel = InLevel;

	ClassType = EClassType::CT_None;
	InitMonsterStatus(CurrentLevel);
}


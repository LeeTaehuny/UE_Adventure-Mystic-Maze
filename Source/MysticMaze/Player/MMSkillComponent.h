// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MMEnums.h"
#include "MMSkillComponent.generated.h"

// 퀵슬롯(포션) 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnChangedSkillSlotDelegate);
// 스킬 쿨타임 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCooltimeChangedDelegate, int32 /* QuickSlot Index */);
// 스킬 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnChangedSkillDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSTICMAZE_API UMMSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMMSkillComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Delegate
public:
	FOnCooltimeChangedDelegate OnCooltimeChanged;
	FOnChangedSkillSlotDelegate OnChangedSkillSlot;
	FOnChangedSkillDelegate OnSkillChanged;

public:
	FORCEINLINE const TArray<TObjectPtr<class UMMSkillBase>>& GetSkillQuickSlot() { return SkillQuickSlots; }
	FORCEINLINE const TArray<TObjectPtr<class UMMSkillBase>>& GetSkillList() { return SkillList; }

	class UMMSkillBase* GetUsingSkill();
	void UseSkill(int32 Index);
	void SetSkillEnd(FString SkillName);
	void AddSkill(FString SkillName);

	// 퀵슬롯 등록
	void SetQuickSlot(ESlotType InPrevSlotType, int32 InPrevIndex, int32 InCurrentIndex);

protected:
	// 스킬 매니저 초기화
	void InitSkillManager();
	// 스킬 리스트 초기화
	void InitSkillList();

	// 현재 배운 스킬 목록
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMSkillBase>> SkillList;
		
	// 퀵슬롯 목록
	UPROPERTY(VisibleAnywhere, Category = "QuickSlot", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMSkillBase>> SkillQuickSlots;

protected:
	// 모든 스킬 목록
	UPROPERTY(VisibleAnywhere, Category = "SkillManager", meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<class UMMSkillData>> SkillManager;

	// 현재 사용중인 스킬을 관리하기 위한 비트플래그
	enum class ESkillFlag
	{
		None = 0,			// 0000 0000 - 초기화			0
		SKILL1 = 1 << 0,	// 0000 0001 - 1번 스킬 사용 중	1
		SKILL2 = 1 << 1,	// 0000 0010 - 2번 스킬 사용 중	2
		SKILL3 = 1 << 2,	// 0000 0100 - 3번 스킬 사용 중	4
		SKILL4 = 1 << 3,    // 0000 1000 - 4번 스킬 사용 중	8
	};

	uint8 SkillCheckFlag;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMSkillComponent.h"
#include "Skill/MMSkillData.h"
#include "Skill/MMSkillBase.h"
#include "Skill/Warrior/MMSkill_ComboSlash.h"
#include "GameData/MMStructures.h"
#include "Game/MMSaveGameData.h"
#include "Game/MMGameInstance.h"

#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

UMMSkillComponent::UMMSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// 초기 사이즈 지정
	SkillList.Init(nullptr, 4);
	SkillQuickSlots.Init(nullptr, 4);

	SkillCheckFlag = static_cast<uint8>(ESkillFlag::None);
}

void UMMSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMMSkillComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 스킬 매니저 초기화
	InitSkillManager();

	// 스킬 리스트 초기화
	InitSkillList();
}


// Called every frame
void UMMSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (const auto& Skill : SkillList)
	{
		if (IsValid(Skill))
		{
			if (Skill->CoolDown(DeltaTime))
			{
				int32 Index = 0;
				for (const auto& QuickSkill : SkillQuickSlots)
				{
					if (IsValid(QuickSkill) && Skill == QuickSkill)
						OnCooltimeChanged.Broadcast(Index);

					Index++;
				}
			}
		}
	}
}

UMMSkillBase* UMMSkillComponent::GetUsingSkill()
{
	// 현재 사용중인 스킬 반환
	switch (SkillCheckFlag)
	{
	case static_cast<uint8>(ESkillFlag::SKILL1):
		if (IsValid(SkillQuickSlots[0]))
		{
			return SkillQuickSlots[0];
		}
		break;

	case static_cast<uint8>(ESkillFlag::SKILL2):
		if (IsValid(SkillQuickSlots[1]))
		{
			return SkillQuickSlots[1];
		}
		break;

	case static_cast<uint8>(ESkillFlag::SKILL3):
		if (IsValid(SkillQuickSlots[2]))
		{
			return SkillQuickSlots[2];
		}
		break;

	case static_cast<uint8>(ESkillFlag::SKILL4):
		if (IsValid(SkillQuickSlots[3]))
		{
			return SkillQuickSlots[3];
		}
		break;
	}

	return nullptr;
}

void UMMSkillComponent::UseSkill(int32 Index)
{
	// 스킬 사용중인 경우 다른 스킬 사용 불가
	if (SkillCheckFlag != static_cast<uint8>(ESkillFlag::None)) return;

	if (SkillQuickSlots.IsValidIndex(Index) && IsValid(SkillQuickSlots[Index]))
	{
		// 스킬을 사용했다면?
		if (SkillQuickSlots[Index]->UseSkill())
		{
			// 해당 인덱스 스킬 사용 체크
			SkillCheckFlag |= static_cast<uint8>(1 << Index);
		}
	}
}

void UMMSkillComponent::SetSkillEnd(FString SkillName)
{
	// 해당 이름의 스킬이 종료됨을 표시합니다.
	int32 Index = 0;
	for (const auto& Skill : SkillQuickSlots)
	{
		if (!IsValid(Skill)) 
		{
			Index++;
			continue;
		}

		// 같은 이름의 스킬이라면?
		if (Skill->GetSkillData()->SkillName == SkillName)
		{
			SkillCheckFlag &= ~static_cast<uint8>(1 << Index);
			break;
		}
		Index++;
	}
}

void UMMSkillComponent::AddSkill(FString SkillName)
{
	// 해당 이름의 스킬이 존재한다면 내 리스트에 추가하기
	if (SkillManager.Find(SkillName))
	{
		UMMSkillBase* NewSkill = NewObject<UMMSkillBase>(this, SkillManager[SkillName]->SkillClass);
		if (NewSkill)
		{
			NewSkill->Init(SkillManager[SkillName], 1, GetOwner());

			for (auto& TempSkill : SkillList)
			{
				if (!IsValid(TempSkill))
				{
					TempSkill = NewSkill;
					OnSkillChanged.Broadcast();
					break;
				}
			}
		}
	}
}

void UMMSkillComponent::SetQuickSlot(ESlotType InPrevSlotType, int32 InPrevIndex, int32 InCurrentIndex)
{
	if (InPrevSlotType == ESlotType::ST_SkillSlot)
	{
		if (SkillList.IsValidIndex(InPrevIndex) && IsValid(SkillList[InPrevIndex]) && SkillQuickSlots.IsValidIndex(InCurrentIndex))
		{
			// 이미 퀵슬롯에 등록되어 있는지 찾기
			int32 Index = 0;
			for (auto& Skill : SkillQuickSlots)
			{
				// 등록된 스킬이라면?
				if (Skill == SkillList[InPrevIndex])
				{
					// 현재 사용중인 스킬이라면 교체 불가
					if (SkillCheckFlag & (1 << Index))
					{
						return;
					}

					// 인덱스를 교환해주고 종료합니다.
					SkillQuickSlots[InCurrentIndex] = SkillQuickSlots[Index];
					SkillQuickSlots[Index] = nullptr;
					OnChangedSkillSlot.Broadcast();
					return;
				}
				Index++;
			}
			// 등록되지 않은 스킬인 경우 등록해주도록 합니다.
			SkillQuickSlots[InCurrentIndex] = SkillList[InPrevIndex];
			OnChangedSkillSlot.Broadcast();
		}
	}
}

void UMMSkillComponent::SaveSkill()
{
	// GameInstance에서 Save파일 이름 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	// 게임 데이터 인스턴스 받아오기
	UMMSaveGameData* GameData = GameInstance->GetSaveData();
	if (GameData)
	{
		// 스킬 데이터 및 퀵슬롯 저장
		{
			TArray<FMMSkillSaveData> SaveSkills;
			for (int i = 0; i < SkillList.Num(); i++)
			{
				// 스킬이 존재한다면?
				if (IsValid(SkillList[i]))
				{
					FMMSkillSaveData SaveData;
					SaveData.SkillName = SkillList[i]->GetSkillData()->GetName();
					SaveData.SkillLevel = SkillList[i]->GetSkillLevel();
					SaveData.QuickSlotIndex = -1;

					for (int j = 0; j < SkillQuickSlots.Num(); j++)
					{
						if (IsValid(SkillQuickSlots[j]))
						{
							// 같은 스킬이 퀵슬롯에 존재한다면?
							if (SkillList[i] == SkillQuickSlots[j])
							{
								// 스킬 인덱스 저장
								SaveData.QuickSlotIndex = j;
							}
						}
					}

					SaveSkills.Add(SaveData);
				}
			}
			// 세이브 파일에 저장
			GameData->SkillData = SaveSkills;
		}

		// 게임 저장하기
		UGameplayStatics::SaveGameToSlot(GameData, GameData->SaveSlotName, GameData->SaveIndex);
	}
}

void UMMSkillComponent::InitSkillManager()
{
	// 애셋 매니저 불러오기
	UAssetManager& Manager = UAssetManager::Get();

	// 애셋 아이디 리스트 받아오기
	TArray<FPrimaryAssetId> Assets;
	// * 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 반환받음
	Manager.GetPrimaryAssetIdList(TEXT("MMSkillData"), Assets);

	if (Assets.Num() > 0)
	{
		// 모든 스킬 추가하기
		for (int i = 0; i < Assets.Num(); i++)
		{
			FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[i]));
			if (AssetPtr.IsPending())
			{
				AssetPtr.LoadSynchronous();
			}

			UMMSkillData* Skill = Cast<UMMSkillData>(AssetPtr.Get());
			if (Skill)
			{
				SkillManager.Add(AssetPtr->GetName(), Skill);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AssetPtr->GetName());
			}
		}
	}
}

void UMMSkillComponent::InitSkillList()
{
	// GameInstance에서 Save파일 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	UMMSaveGameData* GameData = GameInstance->GetSaveData();
	// 저장된 게임 데이터가 있는 경우라면?
	if (GameData)
	{
		// 파일로부터 읽어오기
		TArray<FMMSkillSaveData> SkillSaveData = GameData->SkillData;

		for (const auto& Skill : SkillSaveData)
		{
			// 해당 이름의 스킬이 존재한다면 내 리스트에 추가하기
			if (SkillManager.Find(Skill.SkillName))
			{
				UMMSkillBase* NewSkill = NewObject<UMMSkillBase>(this, SkillManager[Skill.SkillName]->SkillClass);
				if (NewSkill)
				{
					NewSkill->Init(SkillManager[Skill.SkillName], Skill.SkillLevel, GetOwner());
					for (auto& TempSkill : SkillList)
					{
						if (!IsValid(TempSkill))
						{
							TempSkill = NewSkill;
							break;
						}
					}

					// 퀵슬롯에 등록되어 있다면 자동으로 추가하기
					if (Skill.QuickSlotIndex != -1)
					{
						SkillQuickSlots[Skill.QuickSlotIndex] = NewSkill;
					}
				}
			}
		}
	}
}


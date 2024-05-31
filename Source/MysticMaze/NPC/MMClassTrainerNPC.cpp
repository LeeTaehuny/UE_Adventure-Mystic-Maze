// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/MMClassTrainerNPC.h"
#include "UI/NPC/MMClassTrainerWidget.h"
#include "UI/MMHUDWidget.h"
#include "Player/MMPlayerController.h"
#include "Skill/MMSkillData.h"
#include "Skill/MMSkillBase.h"
#include "Interface/MMSkillInterface.h"
#include "Interface/MMStatusInterface.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMSkillComponent.h"
#include "Player/MMStatComponent.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"

AMMClassTrainerNPC::AMMClassTrainerNPC()
{
	HelpText = TEXT("Press F to Interaction the ClassTrainer.");
	PlayerLevel = 0;
}

void AMMClassTrainerNPC::BeginPlay()
{
	Super::BeginPlay();
}

void AMMClassTrainerNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMMClassTrainerNPC::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AMMClassTrainerNPC::OnEndOverlap);

	// 스킬 맵 생성
	for (const auto& SkillData : WarriorSkillData)
	{
		if (SkillData)
		{
			WarriorSkillMap.Add(SkillData->GetName(), 0);
		}
	}

	for (const auto& SkillData : ArcherSkillData)
	{
		if (SkillData)
		{
			ArcherSkillMap.Add(SkillData->GetName(), 0);
		}
	}

	for (const auto& SkillData : MageSkillData)
	{
		if (SkillData)
		{
			MageSkillMap.Add(SkillData->GetName(), 0);
		}
	}
}

void AMMClassTrainerNPC::CloseShop()
{
	if (TempPlayer && ClassTrainerWidget)
	{
		// 게임모드로 전환
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(TempPlayer->GetController());
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = false;
			ClassTrainerWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		TempPlayer = nullptr;
	}
}

void AMMClassTrainerNPC::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->GetHUDWidget()->InteractionWidgetHelpText(HelpText);
			PlayerController->GetHUDWidget()->ToggleInteractionWidget(true);
		}
	}
}

void AMMClassTrainerNPC::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 비활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->GetHUDWidget()->ToggleInteractionWidget(false);
		}
	}
}

void AMMClassTrainerNPC::Interaction(ACharacter* PlayerCharacter)
{
	TempPlayer = PlayerCharacter;

	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
	
	// 스킬 맵 업데이트
	UpdateSkillMap();

	if (!ClassTrainerWidget)
	{
		if (ClassTrainerWidgetClass)
		{
			ClassTrainerWidget = CreateWidget<UMMClassTrainerWidget>(GetWorld(), ClassTrainerWidgetClass);

			// 위젯 초기화
			ClassTrainerWidget->SetOwningActor(this);
			ClassTrainerWidget->Init(PlayerCharacter);
			ClassTrainerWidget->AddToViewport();
		}
	}
	else
	{
		ClassTrainerWidget->UpdateWidget();
		ClassTrainerWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayerController)
	{
		// UI모드로 설정
		FInputModeUIOnly InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void AMMClassTrainerNPC::UpdateSkillMap()
{
	if (TempPlayer)
	{
		IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(TempPlayer);
		if (StatPlayer)
		{
			// 레벨 설정
			PlayerLevel = StatPlayer->GetStatComponent()->GetCurrentLevel();

			switch (StatPlayer->GetStatComponent()->GetClass())
			{
			case EClassType::CT_Beginner:
				return;
				break;

			case EClassType::CT_Warrior:
				UpdateWarriorSkillMap();
				break;

			case EClassType::CT_Archer:
				UpdateArcherSkillMap();
				break;

			case EClassType::CT_Mage:
				UpdateMageSkillMap();
				break;
			}
		}
	}
}

void AMMClassTrainerNPC::UpdateWarriorSkillMap()
{
	if (TempPlayer)
	{
		IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(TempPlayer);
		if (SkillPlayer)
		{
			TArray<UMMSkillBase*> SkillList = SkillPlayer->GetSkillComponent()->GetSkillList();

			for (const auto& Skill : SkillList)
			{
				if (!IsValid(Skill)) continue;

				// 스킬의 이름이 스킬 맵에 존재한다면?
				if (WarriorSkillMap.Find(Skill->GetSkillData()->GetName()))
				{
					// 해당 스킬의 레벨 업데이트
					WarriorSkillMap[Skill->GetSkillData()->GetName()] = Skill->GetSkillLevel();
				}
			}
		}
	}
}

void AMMClassTrainerNPC::UpdateArcherSkillMap()
{
	if (TempPlayer)
	{
		IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(TempPlayer);
		if (SkillPlayer)
		{
			TArray<UMMSkillBase*> SkillList = SkillPlayer->GetSkillComponent()->GetSkillList();

			for (const auto& Skill : SkillList)
			{
				if (!IsValid(Skill)) continue;

				// 스킬의 이름이 스킬 맵에 존재한다면?
				if (ArcherSkillMap.Find(Skill->GetSkillData()->GetName()))
				{
					// 해당 스킬의 레벨 업데이트
					ArcherSkillMap[Skill->GetSkillData()->GetName()] = Skill->GetSkillLevel();
				}
			}
		}
	}
}

void AMMClassTrainerNPC::UpdateMageSkillMap()
{
	if (TempPlayer)
	{
		IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(TempPlayer);
		if (SkillPlayer)
		{
			TArray<UMMSkillBase*> SkillList = SkillPlayer->GetSkillComponent()->GetSkillList();

			for (const auto& Skill : SkillList)
			{
				if (!IsValid(Skill)) continue;

				// 스킬의 이름이 스킬 맵에 존재한다면?
				if (MageSkillMap.Find(Skill->GetSkillData()->GetName()))
				{
					// 해당 스킬의 레벨 업데이트
					MageSkillMap[Skill->GetSkillData()->GetName()] = Skill->GetSkillLevel();
				}
			}
		}
	}
}

void AMMClassTrainerNPC::Purchase(ESlotType Type, int32 ItemIndex)
{
	if (TempPlayer && ClassTrainerWidget)
	{
		UMMSkillData* SkillData = nullptr;

		// 해당 스킬 데이터 찾기
		switch (Type)
		{
		case ESlotType::ST_ClassSkillWarrior:
			if (WarriorSkillData.IsValidIndex(ItemIndex) && IsValid(WarriorSkillData[ItemIndex]))
			{
				SkillData = WarriorSkillData[ItemIndex];
			}
			break;

		case ESlotType::ST_ClassSkillArcher:
			if (ArcherSkillData.IsValidIndex(ItemIndex) && IsValid(ArcherSkillData[ItemIndex]))
			{
				SkillData = ArcherSkillData[ItemIndex];
			}
			break;

		case ESlotType::ST_ClassSkillMage:
			if (MageSkillData.IsValidIndex(ItemIndex) && IsValid(MageSkillData[ItemIndex]))
			{
				SkillData = MageSkillData[ItemIndex];
			}
			break;
		}

		IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(TempPlayer);
		IMMSkillInterface* SkillPlayer = Cast<IMMSkillInterface>(TempPlayer);
		if (!InvPlayer) return;
		if (!SkillPlayer) return;

		// 현재 플레이어의 스킬 리스트에서 해당 스킬 찾기
		TArray<UMMSkillBase*> SkillList = SkillPlayer->GetSkillComponent()->GetSkillList();
		for (const auto& Skill : SkillList)
		{
			if (!IsValid(Skill)) continue;

			// 존재한다면? 스킬 레벨 올리기 + 돈 소모하기
			if (Skill->GetSkillData()->SkillName == SkillData->SkillName)
			{
				// 돈 소모가 가능하다면?
				if (InvPlayer->GetInventoryComponent()->UseGold(SkillData->UpgradeGold[Skill->GetSkillLevel()]))
				{
					// 스킬 레벨 증가
					Skill->SetSkillLevel(Skill->GetSkillLevel() + 1);
					// 스킬 맵 업데이트 하기
					UpdateSkillMap();
					// 위젯 업데이트 하기
					ClassTrainerWidget->UpdateWidget();
					// 스킬 위젯 업데이트
					SkillPlayer->GetSkillComponent()->OnSkillChanged.Broadcast();
					return;
				}
				else
				{
					return;
				}
			}
		}

		// 찾지 못한 경우 돈 소모하고 스킬 추가하기
		if (InvPlayer->GetInventoryComponent()->UseGold(SkillData->UpgradeGold[0]))
		{
			SkillPlayer->GetSkillComponent()->AddSkill(SkillData->GetName());
			// 스킬 맵 업데이트 하기
			UpdateSkillMap();
			// 위젯 업데이트 하기
			ClassTrainerWidget->UpdateWidget();
			return;
		}
	}
}
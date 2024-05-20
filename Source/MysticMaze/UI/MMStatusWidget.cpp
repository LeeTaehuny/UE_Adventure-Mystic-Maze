// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMStatusWidget.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UMMStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsDragging)
	{
		FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		float DeltaX = InitialOffset.X - MousePos.X;
		float DeltaY = InitialOffset.Y - MousePos.Y;

		InitialPos.X += -DeltaX;
		InitialPos.Y += -DeltaY;

		InitialOffset = MousePos;

		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Border);
		if (slot)
		{
			slot->SetPosition(InitialPos);
		}
	}
}

void UMMStatusWidget::Init()
{
	bIsDragging = false;

	TArray<UWidget*> TempWidgets;
	WidgetTree->GetAllWidgets(TempWidgets);

	for (UWidget* widget : TempWidgets)
	{
		UBorder* border = Cast<UBorder>(widget);
		if (border)
		{
			Border = border;
			break;
		}
	}

	if (BTN_MainButton)
	{
		BTN_MainButton->OnPressed.AddDynamic(this, &UMMStatusWidget::MoveStart);
		BTN_MainButton->OnReleased.AddDynamic(this, &UMMStatusWidget::MoveEnd);
	}

	if (BTN_UpgradeSTR)
	{
		BTN_UpgradeSTR->OnClicked.AddDynamic(this, &UMMStatusWidget::UpgradeSTR);
	}

	if (BTN_UpgradeDEX)
	{
		BTN_UpgradeDEX->OnClicked.AddDynamic(this, &UMMStatusWidget::UpgradeDEX);
	}

	if (BTN_UpgradeCON)
	{
		BTN_UpgradeCON->OnClicked.AddDynamic(this, &UMMStatusWidget::UpgradeCON);
	}

	if (BTN_UpgradeINT)
	{
		BTN_UpgradeINT->OnClicked.AddDynamic(this, &UMMStatusWidget::UpgradeINT);
	}

	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	UpdateStat(StatPlayer->GetStatComponent()->GetTotalStat());
}

void UMMStatusWidget::UpdateStat(const FMMCharacterStat& InTotalStat)
{
	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	if (!StatPlayer) return;

	// BaseStat Update
	TXT_TotalSTR->SetText(FText::FromString(FString::Printf(TEXT("%d"), InTotalStat.STR)));
	TXT_TotalDEX->SetText(FText::FromString(FString::Printf(TEXT("%d"), InTotalStat.DEX)));
	TXT_TotalCON->SetText(FText::FromString(FString::Printf(TEXT("%d"), InTotalStat.CON)));
	TXT_TotalINT->SetText(FText::FromString(FString::Printf(TEXT("%d"), InTotalStat.INT)));

	// DetailStat Update
	TXT_MaxHp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), StatPlayer->GetStatComponent()->GetMaxHp())));
	TXT_MaxMp->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), StatPlayer->GetStatComponent()->GetMaxMp())));
	TXT_AttackDamage->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), StatPlayer->GetStatComponent()->GetAttackDamage())));
	TXT_Defense->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), StatPlayer->GetStatComponent()->GetDefense())));
	TXT_MovementSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), StatPlayer->GetStatComponent()->GetMovementSpeed())));
	TXT_AttackSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), StatPlayer->GetStatComponent()->GetAttackSpeed())));
	TXT_CriticalHitRate->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), StatPlayer->GetStatComponent()->GetCriticalHitRate())));

	// Level & StatPoint Update
	TXT_CurrentLevel->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatPlayer->GetStatComponent()->GetCurrentLevel())));
	TXT_StatPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatPlayer->GetStatComponent()->GetAvailableStatPoint())));

	// Class Update
	FString ClassName;

	switch (StatPlayer->GetStatComponent()->GetClass())
	{
	case EClassType::CT_Beginner:
		ClassName = TEXT("Beginner");
		break;

	case EClassType::CT_Warrior:
		ClassName = TEXT("Warrior");
		break;

	case EClassType::CT_Archer:
		ClassName = TEXT("Archer");
		break;

	case EClassType::CT_Mage:
		ClassName = TEXT("Mage");
		break;
	}

	TXT_Class->SetText(FText::FromString(ClassName));

	// Button Setting (스탯 잔여 포인트에 따른 활성화 / 비활성화)
	if (StatPlayer->GetStatComponent()->GetAvailableStatPoint() > 0)
	{
		BTN_UpgradeSTR->SetIsEnabled(true);
		BTN_UpgradeDEX->SetIsEnabled(true);
		BTN_UpgradeCON->SetIsEnabled(true);
		BTN_UpgradeINT->SetIsEnabled(true);
	}
	else
	{
		BTN_UpgradeSTR->SetIsEnabled(false);
		BTN_UpgradeDEX->SetIsEnabled(false);
		BTN_UpgradeCON->SetIsEnabled(false);
		BTN_UpgradeINT->SetIsEnabled(false);
	}
}

void UMMStatusWidget::MoveStart()
{
	bIsDragging = true;

	FVector2D WidgetPos;

	UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Border);
	if (slot)
	{
		WidgetPos = slot->GetPosition();
	}

	InitialOffset = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	InitialPos = WidgetPos;
}

void UMMStatusWidget::MoveEnd()
{
	bIsDragging = false;
}

void UMMStatusWidget::UpgradeSTR()
{
	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	if (StatPlayer)
	{
		StatPlayer->GetStatComponent()->UpgradeStat(EStatusType::ST_STR);
	}
}

void UMMStatusWidget::UpgradeDEX()
{
	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	if (StatPlayer)
	{
		StatPlayer->GetStatComponent()->UpgradeStat(EStatusType::ST_DEX);
	}
}

void UMMStatusWidget::UpgradeCON()
{
	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	if (StatPlayer)
	{
		StatPlayer->GetStatComponent()->UpgradeStat(EStatusType::ST_CON);
	}
}

void UMMStatusWidget::UpgradeINT()
{
	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	if (StatPlayer)
	{
		StatPlayer->GetStatComponent()->UpgradeStat(EStatusType::ST_INT);
	}
}

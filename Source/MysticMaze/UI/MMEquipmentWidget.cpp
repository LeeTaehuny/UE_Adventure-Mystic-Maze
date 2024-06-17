// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMEquipmentWidget.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"
#include "UI/MMSlot.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UMMEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMEquipmentWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UMMEquipmentWidget::Init()
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
		BTN_MainButton->OnPressed.AddDynamic(this, &UMMEquipmentWidget::MoveStart);
		BTN_MainButton->OnReleased.AddDynamic(this, &UMMEquipmentWidget::MoveEnd);
	}

	// 슬롯 초기화
	EquipmentSlot->SetOwningActor(OwningActor);
	EquipmentSlot->SetType(ESlotType::ST_Equipment);
	EquipmentSlot->Init();

	IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(OwningActor);
	UpdateStat(StatPlayer->GetStatComponent()->GetWeaponStat());
}

void UMMEquipmentWidget::UpdateStat(const FMMCharacterStat& InWeaponStat)
{
	// 슬롯 업데이트
	EquipmentSlot->UpdateSlot();

	// 세부 스탯 업데이트
	TXT_WeaponSTR->SetText(FText::FromString(FString::Printf(TEXT("%d"), InWeaponStat.STR)));
	TXT_WeaponDEX->SetText(FText::FromString(FString::Printf(TEXT("%d"), InWeaponStat.DEX)));
	TXT_WeaponCON->SetText(FText::FromString(FString::Printf(TEXT("%d"), InWeaponStat.CON)));
	TXT_WeaponINT->SetText(FText::FromString(FString::Printf(TEXT("%d"), InWeaponStat.INT)));
}

void UMMEquipmentWidget::MoveStart()
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

void UMMEquipmentWidget::MoveEnd()
{
	bIsDragging = false;
}

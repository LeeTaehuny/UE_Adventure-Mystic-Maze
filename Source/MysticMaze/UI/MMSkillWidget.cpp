// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMSkillWidget.h"
#include "UI/MMSlot.h"
#include "Interface/MMSkillInterface.h"

#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UMMSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMSkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UMMSkillWidget::Init()
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
		BTN_MainButton->OnPressed.AddDynamic(this, &UMMSkillWidget::MoveStart);
		BTN_MainButton->OnReleased.AddDynamic(this, &UMMSkillWidget::MoveEnd);
	}

	Slots.Init(nullptr, 4);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		UMMSlot* InvSlot = Cast<UMMSlot>(Widget);
		if (InvSlot)
		{
			InvSlot->SetOwningActor(OwningActor);
			InvSlot->SetType(ESlotType::ST_SkillSlot);
			InvSlot->Init();
			Slots[InvSlot->SlotIndex] = InvSlot;
		}
	}
}

void UMMSkillWidget::UpdateSkillSlot()
{
	// 슬롯 초기화
	for (const auto& InvSlot : Slots)
	{
		InvSlot->UpdateSlot();
	}
}

void UMMSkillWidget::MoveStart()
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

void UMMSkillWidget::MoveEnd()
{
	bIsDragging = false;
}

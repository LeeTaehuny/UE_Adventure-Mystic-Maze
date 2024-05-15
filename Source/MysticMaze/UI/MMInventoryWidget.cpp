// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMInventoryWidget.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Blueprint/WidgetTree.h"
#include "UI/MMSlot.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UMMInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMMInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UMMInventoryWidget::Init()
{
	bIsDragging = false;
	SetType(ESlotType::ST_InventoryEquipment);

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

	if (BTN_Equipment)
	{
		BTN_Equipment->OnClicked.AddDynamic(this, &UMMInventoryWidget::SetEquipmentType);
	}

	if (BTN_Consumable)
	{
		BTN_Consumable->OnClicked.AddDynamic(this, &UMMInventoryWidget::SetConsumableType);
	}

	if (BTN_Other)
	{
		BTN_Other->OnClicked.AddDynamic(this, &UMMInventoryWidget::SetOtherType);
	}

	if (BTN_MainButton)
	{
		BTN_MainButton->OnPressed.AddDynamic(this, &UMMInventoryWidget::MoveStart);
		BTN_MainButton->OnReleased.AddDynamic(this, &UMMInventoryWidget::MoveEnd);
	}

	if (BTN_SortItem)
	{
		BTN_SortItem->OnClicked.AddDynamic(this, &UMMInventoryWidget::SortItem);
	}

	Slots.Init(nullptr, 30);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		UMMSlot* InvSlot = Cast<UMMSlot>(Widget);
		if (InvSlot)
		{
			InvSlot->SetOwningActor(OwningActor);
			InvSlot->SetType(InventorySlotType);
			InvSlot->Init();
			Slots[InvSlot->SlotIndex] = InvSlot;
		}
	}
}

void UMMInventoryWidget::UpdateInventorySlot()
{
	// 슬롯 초기화
	for (const auto& InvSlot : Slots)
	{
		InvSlot->SetType(InventorySlotType);
		InvSlot->UpdateSlot();
	}
}

void UMMInventoryWidget::UpdateInventoryGold()
{
	UE_LOG(LogTemp, Warning, TEXT("GoldUpdate"));

	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
	if (InvPlayer)
	{
		TXT_Gold->SetText(FText::FromString(FString::Printf(TEXT("%d"), InvPlayer->GetInventoryComponent()->GetCurrentGold())));
	}
}

void UMMInventoryWidget::SetEquipmentType()
{
	SetType(ESlotType::ST_InventoryEquipment);
}

void UMMInventoryWidget::SetConsumableType()
{
	SetType(ESlotType::ST_InventoryConsumable);
}

void UMMInventoryWidget::SetOtherType()
{
	SetType(ESlotType::ST_InventoryOther);
}

void UMMInventoryWidget::MoveStart()
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

void UMMInventoryWidget::MoveEnd()
{
	bIsDragging = false;
}

void UMMInventoryWidget::SortItem()
{
	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(OwningActor);
	if (InvPlayer)
	{
		InvPlayer->GetInventoryComponent()->SortItem(InventorySlotType);
	}
}

void UMMInventoryWidget::SetType(ESlotType Type)
{
	InventorySlotType = Type;

	UpdateInventorySlot();
}

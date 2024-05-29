// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "GameData/MMEnums.h"
#include "MMDragSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMDragSlot : public UDragDropOperation
{
	GENERATED_BODY()
	
// Drag 정보 저장용 변수
public:
	UPROPERTY(VisibleAnywhere)
	int PrevSlotIndex;

	UPROPERTY(VisibleAnywhere)
	ESlotType SlotType;
};

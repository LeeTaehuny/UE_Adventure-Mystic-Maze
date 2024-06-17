
#pragma once

#include "CoreMinimal.h"
#include "MMStructures.generated.h"

USTRUCT(BlueprintType)
struct FMMSkillSaveData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    int32 SkillLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    int32 QuickSlotIndex;
};

USTRUCT(BlueprintType)
struct FMMItemSaveData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    int32 SlotIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    int32 ItemQuantity;
};
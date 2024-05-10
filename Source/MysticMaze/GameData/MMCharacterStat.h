﻿
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MMCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FMMCharacterStat : public FTableRowBase
{
    GENERATED_BODY()

public:
    FMMCharacterStat() : STR(0), DEX(0), CON(0), INT(0) {}

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
        int32 STR;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
        int32 DEX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
        int32 CON;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
        int32 INT;

    FMMCharacterStat operator+(const FMMCharacterStat& other) const
    {
        FMMCharacterStat Result;
      
        Result.STR = this->STR + other.STR;
        Result.DEX = this->DEX + other.DEX;
        Result.CON = this->CON + other.CON;
        Result.INT = this->INT + other.INT;

        return Result;
    }
};
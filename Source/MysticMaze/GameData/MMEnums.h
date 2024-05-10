
#pragma once

#include "CoreMinimal.h"
#include "MMEnums.generated.h"

UENUM(BlueprintType)
enum class EClassType : uint8
{
    CT_None,
    CT_Beginner,    // 초보자
    CT_Warrior,     // 전사
    CT_Archer,      // 궁수
    CT_Mage,        // 마법사
};
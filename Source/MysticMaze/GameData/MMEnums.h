
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

UENUM(BlueprintType)
enum class ESlotType : uint8
{
    ST_None,
    ST_InventoryEquipment,  // 인벤토리(장비)
    ST_InventoryConsumable, // 인벤토리(소비)
    ST_InventoryOther,      // 인벤토리(기타)
    ST_SkillSlot,           // 스킬 슬롯 (스킬창)
    ST_SkillQuickSlot,      // 스킬 슬롯 (퀵슬롯)
    ST_PotionSlot,          // 포션 슬롯 (퀵슬롯)
    ST_Equipment,           // 장비 슬롯 (장비창)
    ST_ShopPotion,          // 포션 슬롯 (상점)
    ST_ShopWarriorWeapon,   // 전사 무기 슬롯 (상점)
    ST_ShopArcherWeapon,    // 궁수 무기 슬롯 (상점)
    ST_ShopMageWeapon,      // 법사 무기 슬롯 (상점)
    ST_ClassSkillWarrior,   // 전사 스킬 슬롯 (전직 교관)
    ST_ClassSkillArcher,    // 궁수 스킬 슬롯 (전직 교관)
    ST_ClassSkillMage,      // 법사 스킬 슬롯 (전직 교관)
};
// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMSwordWeapon.h"


AMMSwordWeapon::AMMSwordWeapon()
{
	WeaponType = EWeaponType::WT_Sword;
	BaseSocketName = TEXT("SwordPosition");
	DrawSocketName = TEXT("SwordSocket");
}

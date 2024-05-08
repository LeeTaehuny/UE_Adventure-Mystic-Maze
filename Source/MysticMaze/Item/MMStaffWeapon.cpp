// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMStaffWeapon.h"

AMMStaffWeapon::AMMStaffWeapon()
{
	WeaponType = EWeaponType::WT_Staff;
	BaseSocketName = TEXT("StaffPosition");
	DrawSocketName = TEXT("StaffSocket");
}

void AMMStaffWeapon::BeginPlay()
{
	Super::BeginPlay();
}

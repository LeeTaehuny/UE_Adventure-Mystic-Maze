// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMBowWeapon.h"

#include "GameFramework/Character.h"

AMMBowWeapon::AMMBowWeapon()
{
	WeaponType = EWeaponType::WT_Bow;
	BaseSocketName = TEXT("BowPosition");
	DrawSocketName = TEXT("BowSocket");
	QuiverSocketName = TEXT("QuiverPosition");
}

void AMMBowWeapon::EquipWeapon(ACharacter* Player)
{
	Super::EquipWeapon(Player);

	if (QuiverClass)
	{
		FActorSpawnParameters Params;
		Quiver = GetWorld()->SpawnActor(QuiverClass);

		if (Quiver)
		{
			USkeletalMeshComponent* PlayerMesh = Player->GetMesh();

			Quiver->AttachToComponent(PlayerMesh, FAttachmentTransformRules::KeepRelativeTransform, QuiverSocketName);
		}
	}
}

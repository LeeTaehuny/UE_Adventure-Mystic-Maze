// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMSwordWeapon.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"

AMMSwordWeapon::AMMSwordWeapon()
{
	WeaponType = EWeaponType::WT_Sword;
	BaseSocketName = TEXT("SwordPosition");
	DrawSocketName = TEXT("SwordSocket");

	// Weapon Collision
	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(RootComponent);
	WeaponCollision->SetCollisionProfileName(MMWEAPON);
}

void AMMSwordWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ���۰� ���ÿ� �浹 üũ ����
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMMSwordWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Event Mapping
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMSwordWeapon::OnBeginOverlap);
}

void AMMSwordWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner == OtherActor) return;

	// TODO : ������ ����
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
}

void AMMSwordWeapon::EnableCollision()
{
	// �浹 üũ On
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMMSwordWeapon::DisableCollision()
{
	// �浹 üũ Off
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
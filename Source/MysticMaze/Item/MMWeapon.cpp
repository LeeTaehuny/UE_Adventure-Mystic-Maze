// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMWeapon.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMMWeapon::AMMWeapon()
{
	// Component Setting
	{
		WeaponMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("WeaponMesh"));
		RootComponent = WeaponMesh;
		WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

		WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
		WeaponCollision->SetupAttachment(RootComponent);
		WeaponCollision->SetCollisionProfileName(MMWEAPON);
		WeaponCollision->bHiddenInGame = false;
	}
}

// Called when the game starts or when spawned
void AMMWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// 시작과 동시에 충돌 체크 해제
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMMWeapon::EquipWeapon(ACharacter* Player)
{
	if (Player)
	{
		USkeletalMeshComponent* PlayerMesh = Player->GetMesh();

		WeaponMesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::KeepRelativeTransform, BaseSocketName);
	}
}

void AMMWeapon::DrawWeapon(USkeletalMeshComponent* Mesh)
{
	if (Mesh)
	{
		WeaponMesh->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, DrawSocketName);
	}
}

void AMMWeapon::SheatheWeapon(USkeletalMeshComponent* Mesh)
{
	if (Mesh)
	{
		WeaponMesh->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, BaseSocketName);
	}
}

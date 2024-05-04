// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMWeapon.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
AMMWeapon::AMMWeapon()
{
	// Component Setting
	{
		WeaponMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("WeaponMesh"));
		RootComponent = WeaponMesh;
		WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

// Called when the game starts or when spawned
void AMMWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMMWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AMMWeapon::EquipWeapon()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
	if (PlayerCharacter)
	{
		WeaponMesh->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, BaseSocketName);

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
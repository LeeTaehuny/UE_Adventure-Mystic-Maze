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
	}
}

// Called when the game starts or when spawned
void AMMWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// 시작과 동시에 충돌 체크 해제
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMMWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Event Mapping
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMWeapon::OnBeginOverlap);
}

void AMMWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner == OtherActor) return;

	// TODO : Attack 로직 작성하기
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
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

void AMMWeapon::EnableCollision()
{
	// 충돌 체크 On
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMMWeapon::DisableCollision()
{
	// 충돌 체크 Off
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

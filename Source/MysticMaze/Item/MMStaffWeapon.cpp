// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMStaffWeapon.h"
#include "Item/MMEnergyBall.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

AMMStaffWeapon::AMMStaffWeapon()
{
	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor>EnergyBallRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Items/Weapons/BP_EnergyBall.BP_EnergyBall_C'"));
	if (EnergyBallRef.Succeeded())
	{
		EnergyBallClass = EnergyBallRef.Class;
	}

	WeaponType = EWeaponType::WT_Staff;
	BaseSocketName = TEXT("StaffPosition");
	DrawSocketName = TEXT("StaffSocket");
	EnergyBallSocket = TEXT("EnergyBallSocket");
}

void AMMStaffWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMMStaffWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMMStaffWeapon::SpawnEnergyBall()
{
	TempEnergyBall = Cast<AMMEnergyBall>(GetWorld()->SpawnActor(EnergyBallClass));

	if (TempEnergyBall)
	{
		// 에너지 볼을 소켓에 부착합니다.
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
		if (PlayerCharacter)
		{
			TempEnergyBall->SetOwner(Owner);
			TempEnergyBall->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EnergyBallSocket);
		}

		// 발사할 위치를 선정합니다.
		SetFireLocation();
	}
}

void AMMStaffWeapon::ShootEnergyBall()
{
	if (TempEnergyBall)
	{
		// 부모 액터로부터 부착 해제
		TempEnergyBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		TempEnergyBall->Fire(FireLocation);
		TempEnergyBall = nullptr;
	}
}

void AMMStaffWeapon::SetFireLocation()
{
	// 플레이어의 카메라에서 화면의 중앙으로 LineTrace를 진행합니다.
	IMMPlayerVisualInterface* PlayerCharacter = Cast<IMMPlayerVisualInterface>(GetOwner());
	if (PlayerCharacter)
	{
		// 충돌 결과 반환용
		FHitResult HitResult;
		// 시작 지점 (카메라의 위치)
		FVector Start = PlayerCharacter->GetPlayerCamera()->GetComponentLocation();
		// 종료 지점 (카메라 위치 + 카메라 전방벡터 * 20000)
		float Distance = 20000;
		FVector End = Start + (PlayerCharacter->GetPlayerCamera()->GetForwardVector() * Distance);
		// 파라미터 설정
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Shoot), false, Owner);

		// 충돌 탐지
		bool bHasHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			CHANNEL_VISIBILITY,
			Params
		);

		if (bHasHit)
		{
			FireLocation = HitResult.ImpactPoint;
		}
		else
		{
			FireLocation = End;
		}

		// 플레이어를 회전시켜줍니다.
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FRotator PlayerRotator = Character->GetActorRotation();
			Character->SetActorRotation(FRotator(PlayerRotator.Pitch, Character->GetControlRotation().Yaw, PlayerRotator.Roll));
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMBowWeapon.h"
#include "Item/MMArrow.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Camera/CameraComponent.h"

AMMBowWeapon::AMMBowWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor>ArrowRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Items/Weapons/BP_BasicArrow.BP_BasicArrow_C'"));
	if (ArrowRef.Succeeded())
	{
		ArrowClass = ArrowRef.Class;
	}

	WeaponType = EWeaponType::WT_Bow;
	BaseSocketName = TEXT("BowPosition");
	DrawSocketName = TEXT("BowSocket");
	QuiverSocketName = TEXT("QuiverPosition");
	ArrowSocketName = TEXT("ArrowSocket");

	bIsHold = false;
}

void AMMBowWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// 화살 줄의 로컬 위치를 구해줍니다.
	// * GetTransform().InverseTransformPosition(소켓 위치) : 월드 상의 소켓 위치를 월드변환행렬의 역함수를 곱해 로컬 위치를 구합니다.
	BaseLocation = GetTransform().InverseTransformPosition(WeaponMesh->GetSocketLocation(StringSocketName));
}

void AMMBowWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 활을 장전한 상태이면
	if (bIsHold)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
		if (PlayerCharacter)
		{	
			// 화살 위치를 전달받아 StringLocation에 저장합니다.
			StringLocation = GetArrowSocketLocation(PlayerCharacter->GetMesh());
			// 화살 줄 Bone의 위치를 당겨진 상태로 설정합니다.
			WeaponMesh->SetBoneLocationByName(StringSocketName, StringLocation, EBoneSpaces::WorldSpace);
		}
	}
	else
	{
		// 화살 줄 Bone의 위치를 기본 위치로 설정합니다.
		WeaponMesh->SetBoneLocationByName(StringSocketName, BaseLocation, EBoneSpaces::ComponentSpace);
	}
}

void AMMBowWeapon::EquipWeapon()
{
	Super::EquipWeapon();

	if (QuiverClass)
	{
		FActorSpawnParameters Params;
		Quiver = GetWorld()->SpawnActor(QuiverClass);

		if (Quiver)
		{
			ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
			if (PlayerCharacter)
			{
				Quiver->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, QuiverSocketName);
			}
		}
	}
}

void AMMBowWeapon::SpawnArrow()
{
	TempArrow = Cast<AMMArrow>(GetWorld()->SpawnActor(ArrowClass));

	if (TempArrow)
	{
		// 화살을 화살 소켓에 부착합니다.
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
		if (PlayerCharacter)
		{
			IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(GetOwner());
			if (StatPlayer)
			{
				// 화살 초기화
				TempArrow->SetOwner(Owner);
				// 데미지 설정
				TempArrow->SetDamage(StatPlayer->GetStatComponent()->GetAttackDamage());
				// 화살 부착
				TempArrow->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArrowSocketName);
			}
		}
	}
}

void AMMBowWeapon::SpawnArrow(TSubclassOf<AActor> InArrowClass, float InDamage)
{
	TempArrow = Cast<AMMArrow>(GetWorld()->SpawnActor(InArrowClass));

	if (TempArrow)
	{
		// 화살을 화살 소켓에 부착합니다.
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
		if (PlayerCharacter)
		{
			// 화살 초기화
			TempArrow->SetOwner(Owner);
			// 데미지 설정
			TempArrow->SetDamage(InDamage);
			// 화살 부착
			TempArrow->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArrowSocketName);
		}
	}
}

void AMMBowWeapon::ShootArrow()
{
	if (TempArrow)
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
			
			// 부모 액터로부터 부착 해제
			TempArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			if (bHasHit)
			{
				TempArrow->Fire(HitResult.ImpactPoint);
				TempArrow = nullptr;
			}
			else
			{
				TempArrow->Fire(End);
				TempArrow = nullptr;
			}

			// 디버깅
			//FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;
			//DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 3.0f);
		}
	}
}

void AMMBowWeapon::DestroyArrow()
{
	if (TempArrow)
	{
		TempArrow->Destroy();
	}
}

FVector AMMBowWeapon::GetArrowSocketLocation(USkeletalMeshComponent* Mesh)
{
	// 화살 소켓 위치를 반환합니다.
	return Mesh->GetSocketLocation(ArrowSocketName);
}

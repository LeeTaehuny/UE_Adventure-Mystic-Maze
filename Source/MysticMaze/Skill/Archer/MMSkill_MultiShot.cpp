// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Archer/MMSkill_MultiShot.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Item/MMBowWeapon.h"
#include "Collision/MMCollision.h"
#include "Item/MMArrow.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

UMMSkill_MultiShot::UMMSkill_MultiShot()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Archer/AM_MultiShot.AM_MultiShot'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FClassFinder<AActor> ArrowClassRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Items/Weapons/BP_SkillArrow.BP_SkillArrow_C'"));
	if (ArrowClassRef.Class)
	{
		ArrowClass = ArrowClassRef.Class;
	}
}

bool UMMSkill_MultiShot::UseSkill()
{
	bool Result = Super::UseSkill();

	if (!Result) return false;

	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 플레이어 이동 막기
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// 애님 몽타주 재생
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(SkillMontage);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UMMSkill_MultiShot::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);

		// 플레이어 시야 조정
		IMMPlayerVisualInterface* VisiblePlayer = Cast<IMMPlayerVisualInterface>(Owner);
		if (VisiblePlayer)
		{
			VisiblePlayer->GetSpringArm()->TargetArmLength = 100;
			VisiblePlayer->GetPlayerCamera()->SetRelativeLocation(FVector(0.0f, 50.0f, 100.0f));
		}
	}

	return Result;
}

void UMMSkill_MultiShot::SkillAttack()
{
	Super::SkillAttack();

	IMMAnimationWeaponInterface* BowPlayer = Cast<IMMAnimationWeaponInterface>(Owner);
	if (BowPlayer)
	{
		AMMBowWeapon* Bow = Cast<AMMBowWeapon>(BowPlayer->GetWeapon());
		if (Bow)
		{
			// 보여주기 전용 화살 스폰
			Bow->SpawnArrow(ArrowClass, BaseDamage);
			// 타이머 설정
			ArrowFireTimer.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(ArrowFireTimer, this, &UMMSkill_MultiShot::FireArrow, 1.53f, false);
		}
	}
}

void UMMSkill_MultiShot::Cancel()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 스킬 몽타주 재생 종료하기
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// 타이머 초기화
		ArrowFireTimer.Invalidate();

		// 스킬 종료 알림
		IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
		if (Owner)
		{
			SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
		}

		// 플레이어 시야 조정
		IMMPlayerVisualInterface* VisiblePlayer = Cast<IMMPlayerVisualInterface>(Owner);
		if (VisiblePlayer)
		{
			VisiblePlayer->GetSpringArm()->TargetArmLength = 800;
			VisiblePlayer->GetPlayerCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		}

		// 생성된 화살이 있으면 소멸시키기
		IMMAnimationWeaponInterface* BowPlayer = Cast<IMMAnimationWeaponInterface>(Owner);
		if (BowPlayer)
		{
			AMMBowWeapon* Bow = Cast<AMMBowWeapon>(BowPlayer->GetWeapon());
			if (Bow)
			{
				Bow->DestroyArrow();
			}
		}

		// 위치 초기화
		Positions.Reset();
	}
}

void UMMSkill_MultiShot::SkillEnd(UAnimMontage* Montage, bool IsEnded)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	// 스킬 종료 알림
	IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
	if (Owner)
	{
		SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
	}

	// 플레이어 시야 조정
	IMMPlayerVisualInterface* VisiblePlayer = Cast<IMMPlayerVisualInterface>(Owner);
	if (VisiblePlayer)
	{
		VisiblePlayer->GetSpringArm()->TargetArmLength = 800;
		VisiblePlayer->GetPlayerCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	}

	// 위치 초기화
	Positions.Reset();
}

void UMMSkill_MultiShot::FireArrow()
{
	IMMPlayerVisualInterface* VisiblePlayer = Cast<IMMPlayerVisualInterface>(Owner);
	if (!VisiblePlayer) return;

	// 충돌 결과를 반환하기 위한 배열
	TArray<FOverlapResult> OutHitResults;

	// 공격 반경
	float DetectRadius = 5000.0f;
	// 공격 각도
	float AttackDegree = 60.0f;

	// 파라미터
	FCollisionQueryParams CollisionQueryParam;

	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OutHitResults,
		Owner->GetActorLocation(),
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bHasHit && ArrowClass)
	{
		// 충돌한 액터들의 위치 저장하기
		for (FOverlapResult Result : OutHitResults)
		{
			if (Result.GetActor() == Owner) continue;

			FVector ActorLocation = Result.GetActor()->GetActorLocation();
			FVector DirectionToActor = ActorLocation - Owner->GetActorLocation();
			DirectionToActor.Normalize();

			float AngleToActor = FMath::RadiansToDegrees(acosf(FVector::DotProduct(VisiblePlayer->GetPlayerCamera()->GetForwardVector(), DirectionToActor)));
			if (AngleToActor <= AttackDegree / 2)
			{
				Positions.Add(Result.GetActor()->GetActorLocation());
			}
		}
	}

	// 충돌한 적들이 존재한다면?
	if (Positions.Num() && ArrowClass)
	{
		FTransform FireTransform;

		IMMAnimationWeaponInterface* BowPlayer = Cast<IMMAnimationWeaponInterface>(Owner);
		if (BowPlayer)
		{
			AMMBowWeapon* Bow = Cast<AMMBowWeapon>(BowPlayer->GetWeapon());
			if (Bow)
			{
				AMMArrow* Temp = Bow->GetTempArrow();
				if (Temp)
				{
					// 발사할 위치 저장
					FireTransform = Temp->GetTransform();
					// 화살 삭제
					Bow->DestroyArrow();
				}
			}
		}

		// 적들의 위치를 순회하며 화살을 스폰하고 발사합니다.
		for (const auto& TargetPosition : Positions)
		{
			// 화살을 스폰위치에 스폰합니다.
			AMMArrow* Arrow = GetWorld()->SpawnActor<AMMArrow>(ArrowClass, FireTransform);
			// 초기화합니다.
			Arrow->SetOwner(Owner);
			// 데미지를 설정합니다.
			Arrow->SetDamage(BaseDamage);
			// 발사합니다.
			Arrow->Fire(TargetPosition);
		}
	}
	else
	{
		// 생성된 화살이 있으면 소멸시키기
		IMMAnimationWeaponInterface* BowPlayer = Cast<IMMAnimationWeaponInterface>(Owner);
		if (BowPlayer)
		{
			AMMBowWeapon* Bow = Cast<AMMBowWeapon>(BowPlayer->GetWeapon());
			if (Bow)
			{
				Bow->DestroyArrow();
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Archer/MMSkill_RapidFire.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/MMPlayerController.h"
#include "UI/MMHUDWidget.h"
#include "Item/MMBowWeapon.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

UMMSkill_RapidFire::UMMSkill_RapidFire()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Archer/AM_RapidFire.AM_RapidFire'"));
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

bool UMMSkill_RapidFire::UseSkill()
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
		EndDelegate.BindUObject(this, &UMMSkill_RapidFire::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);

		// 플레이어 시야 조정
		IMMPlayerVisualInterface* VisiblePlayer = Cast<IMMPlayerVisualInterface>(Owner);
		if (VisiblePlayer)
		{
			VisiblePlayer->GetSpringArm()->TargetArmLength = 100;
			VisiblePlayer->GetPlayerCamera()->SetRelativeLocation(FVector(0.0f, 50.0f, 100.0f));
		}

		// 스테이터스바 위젯 감추기
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			if (PlayerController->GetHUDWidget())
			{
				// 스테이터스바 위젯 토글 함수를 호출합니다.
				PlayerController->GetHUDWidget()->ToggleStatusBarWidget();
			}
		}
	}

	return Result;
}

void UMMSkill_RapidFire::SkillAttack()
{
	Super::SkillAttack();

	if (ArrowClass)
	{
		IMMAnimationWeaponInterface* BowPlayer = Cast<IMMAnimationWeaponInterface>(Owner);
		if (BowPlayer)
		{
			AMMBowWeapon* Bow = Cast<AMMBowWeapon>(BowPlayer->GetWeapon());
			if (Bow)
			{
				// 화살 스폰
				Bow->SpawnArrow(ArrowClass, BaseDamage);
				// 타이머 설정
				ArrowFireTimer.Invalidate();
				GetWorld()->GetTimerManager().SetTimer(ArrowFireTimer, this, &UMMSkill_RapidFire::FireArrow, 0.18f, false);
			}
		}
	}
}

void UMMSkill_RapidFire::Cancel()
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

		// 스테이터스바 위젯 출력
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			if (PlayerController->GetHUDWidget())
			{
				// 스테이터스바 위젯 토글 함수를 호출합니다.
				PlayerController->GetHUDWidget()->ToggleStatusBarWidget();
			}
		}
	}
}

void UMMSkill_RapidFire::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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

	// 스테이터스바 위젯 출력
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
	if (PlayerController)
	{
		if (PlayerController->GetHUDWidget())
		{
			// 스테이터스바 위젯 토글 함수를 호출합니다.
			PlayerController->GetHUDWidget()->ToggleStatusBarWidget();
		}
	}
}

void UMMSkill_RapidFire::FireArrow()
{
	IMMAnimationWeaponInterface* BowPlayer = Cast<IMMAnimationWeaponInterface>(Owner);
	if (BowPlayer)
	{
		AMMBowWeapon* Bow = Cast<AMMBowWeapon>(BowPlayer->GetWeapon());
		if (Bow)
		{
			Bow->ShootArrow();
		}
	}
}

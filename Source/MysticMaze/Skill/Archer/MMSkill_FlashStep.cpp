// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Archer/MMSkill_FlashStep.h"
#include "Skill/MMSkillData.h"
#include "Player/MMSkillComponent.h"
#include "Interface/MMSkillInterface.h"
#include "Interface/MMPlayerVisualInterface.h"
#include "Collision/MMCollision.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UMMSkill_FlashStep::UMMSkill_FlashStep()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/MysticMaze/Player/Animations/Skill/Archer/AM_FlashStep.AM_FlashStep'"));
	if (MontageRef.Object)
	{
		SkillMontage = MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UNiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Mana.NS_Dash_Mana'"));
	if (UNiagaraRef.Object)
	{
		NiagaraEffect = UNiagaraRef.Object;
	}

	StepDistance = 900.0f;
}

bool UMMSkill_FlashStep::UseSkill()
{
	bool Result = Super::UseSkill();

	if (!Result) return false;

	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 현재 플레이어의 입력 방향 구해오기
		FVector2D MovementVector;

		IMMPlayerVisualInterface* VisiblePlayer = Cast<IMMPlayerVisualInterface>(Owner);
		if (VisiblePlayer)
		{
			MovementVector = VisiblePlayer->GetMovementVector();
		}

		// 목표 방향 구하기
		FRotator TargetRotation;

		// 컨트롤러의 회전 중 Yaw(Z)를 가져와 저장
		const FRotator Rotation = PlayerCharacter->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 회전(Yaw)을 기반으로 전방 및 오른쪽 방향을 받아오기 (X : 전방, Y : 오른쪽)
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 전방 및 오른쪽 방향을 이용하여 목표 회전 각도 계산
		TargetRotation = (ForwardDirection * MovementVector.X + RightDirection * MovementVector.Y).Rotation();

		// 방향 설정하기
		Owner->SetActorRotation(TargetRotation);

		// 플레이어 이동 막기
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// 애님 몽타주 재생
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(SkillMontage);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UMMSkill_FlashStep::SkillEnd);
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	}

	return Result;
}

void UMMSkill_FlashStep::SkillAttack()
{
	Super::SkillAttack();

	// 이펙트 재생하기
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, Owner->GetActorLocation(), Owner->GetActorRotation());
	
	// 이동 위치 탐색하기
	TArray<FHitResult> Results;
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * StepDistance;
	FCollisionQueryParams Params;

	bool bHasHit = GetWorld()->SweepMultiByChannel(
		Results,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_VISIBILITY,
		FCollisionShape::MakeSphere(30.0f),
		Params
	);

	// 최종 텔레포트 위치
	FVector TeleportLocation = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * StepDistance);

	// 충돌이 있었다면?
	if (bHasHit)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (!PlayerCharacter) return;

		FVector MaxDistance = Owner->GetActorLocation();
		bool IsRun = false;
		bool IsHit = false;

		for (FHitResult Result : Results)
		{
			// 충돌한 액터가 플레이어라면 건너뛰기
			if (Result.GetActor() == Owner) continue;

			// 충돌한 액터가 몬스터인 경우라면?
			if (Cast<ACharacter>(Result.GetActor()))
			{
				// 아직 아무도 공격하지 않은 상태라면?
				if (!IsHit)
				{
					// 공격 체크하기
					IsHit = true;

					// 치명타 체크하기
					bool Critical = FMath::FRand() < (CriticalRate / 100);
					UGameplayStatics::ApplyDamage(Result.GetActor(), Critical ? BaseDamage * 2.0f : BaseDamage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
				}
			}
			// 충돌한 액터가 사물이라면?
			else
			{
				// 충돌한 액터까지의 거리가 기존 텔레포트 거리보다 길다면?
				if ((MaxDistance - Owner->GetActorLocation()).Length() < (Result.ImpactPoint - Owner->GetActorLocation()).Length())
				{
					// 최종 위치 반영
					MaxDistance = Result.ImpactPoint;
					IsRun = true;
				}
			}
		}

		// 반영된 위치가 존재한다면?
		if (IsRun)
		{
			// 반영된 위치로 플레이어 순간이동 시키기
			Owner->TeleportTo(MaxDistance, Owner->GetActorRotation(), false, true);
		}
		// 반영된 위치가 존재하지 않는다면?
		else
		{
			// 기존 위치로 플레이어 순간이동 시키기
			Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), false, true);
		}
	}
	else
	{
		// 플레이어 순간이동 시키기
		Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), false, true);
	}
}

void UMMSkill_FlashStep::Cancel()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter && SkillMontage)
	{
		// 스킬 몽타주 재생 종료하기
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, SkillMontage);

		// 플레이어 이동 가능하게 설정
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// 스킬 종료 알림
		IMMSkillInterface* SkillCharacter = Cast<IMMSkillInterface>(Owner);
		if (Owner)
		{
			SkillCharacter->GetSkillComponent()->SetSkillEnd(SkillData->SkillName);
		}
	}
}

void UMMSkill_FlashStep::SkillEnd(UAnimMontage* Montage, bool IsEnded)
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
}

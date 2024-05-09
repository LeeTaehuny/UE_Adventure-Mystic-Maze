// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MMPlayerCharacter.h"
#include "MMComboActionData.h"
#include "Collision/MMCollision.h"
#include "Item/MMWeapon.h"
#include "Item/MMSwordWeapon.h"
#include "Item/MMBowWeapon.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AMMPlayerCharacter::AMMPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Member Variable 초기화
	{
		bIsDash = false;
		bIsRoll = false;
		bIsAttacking = false;
		bIsGuard = false;
		bIsEquip = false;
		bIsChange = false;
		bIsHold = false;
		bCanShoot = false;
		bIsStop = false;
		bIsCharge = false;
		WalkSpeed = 230.0f;
		RunSpeed = 600.0f;

		ClassType = EClassType::CT_None;
	}

	// Collision 설정
	{
		GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);

		// 프리셋 지정
		GetCapsuleComponent()->SetCollisionProfileName(MMCAPSULE);
	}

	// Mesh 설정
	{
		// Load
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Pirate/Mesh_UE5/Full/SKM_Pirate_Full_03.SKM_Pirate_Full_03'"));
		if (SkeletalMeshRef.Object)
		{
			// Mesh 설정
			GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
			
			// Animation 설정
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Player/Animations/ABP_Player.ABP_Player_C'"));
			if (AnimInstanceClassRef.Class)
			{
				GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}

			// Collision 설정
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}

	// Camera 설정
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->TargetArmLength = 500.0f;

		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	}

	// Input
	{
		// 공용
		static ConstructorHelpers::FObjectFinder<UInputAction>IA_DashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_Dash.IA_Dash'"));
		if (IA_DashRef.Object)
		{
			IA_Dash = IA_DashRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_RollRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_Roll.IA_Roll'"));
		if (IA_RollRef.Object)
		{
			IA_Roll = IA_RollRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertWeaponRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertWeapon.IA_ConvertWeapon'"));
		if (IA_ConvertWeaponRef.Object)
		{
			IA_ConvertWeapon = IA_ConvertWeaponRef.Object;
		}

		// Basic Input
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_BasicRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_BasicPlayer.IMC_BasicPlayer'"));
		if (IMC_BasicRef.Object)
		{
			IMC_Array.Add(EClassType::CT_None, IMC_BasicRef.Object);
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_BasicMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Basic/IA_BaseMove.IA_BaseMove'"));
		if (IA_BasicMoveRef.Object)
		{
			IA_BasicMove = IA_BasicMoveRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_BasicLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Basic/IA_BaseLook.IA_BaseLook'"));
		if (IA_BasicLookRef.Object)
		{
			IA_BasicLook = IA_BasicLookRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_BasicAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Basic/IA_BaseAttack.IA_BaseAttack'"));
		if (IA_BasicAttackRef.Object)
		{
			IA_BasicAttack = IA_BasicAttackRef.Object;
		}

		// Warrior Input
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_WarriorRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_WarriorPlayer.IMC_WarriorPlayer'"));
		if (IMC_WarriorRef.Object)
		{
			IMC_Array.Add(EClassType::CT_Warrior, IMC_WarriorRef.Object);
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_WarriorGuardRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Warrior/IA_WarriorGuard.IA_WarriorGuard'"));
		if (IA_WarriorGuardRef.Object)
		{
			IA_WarriorGuard = IA_WarriorGuardRef.Object;
		}

		// Archer Input
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_ArcherRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_ArcherPlayer.IMC_ArcherPlayer'"));
		if (IMC_ArcherRef.Object)
		{
			IMC_Array.Add(EClassType::CT_Archer, IMC_ArcherRef.Object);
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ArcherDrawRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Archer/IA_ArcherDraw.IA_ArcherDraw'"));
		if (IA_ArcherDrawRef.Object)
		{
			IA_ArcherDraw = IA_ArcherDrawRef.Object;
		}

		// Mage Input
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_MageRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_MagePlayer.IMC_MagePlayer'"));
		if (IMC_MageRef.Object)
		{
			IMC_Array.Add(EClassType::CT_Mage, IMC_MageRef.Object);
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_MageSaveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Mage/IA_MageSave.IA_MageSave'"));
		if (IA_MageSaveRef.Object)
		{
			IA_MageSave = IA_MageSaveRef.Object;
		}
	}

	// Setting
	{
		// 컨트롤러의 Rotation에 영향 X
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;

		// 폰의 컨트롤 회전 사용
		SpringArm->bUsePawnControlRotation = true;

		// 움직임에 따른 회전 On
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// 이동속도 조정
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	// Combo Variable 초기화
	{
		CurrentComboCount = 0;
		bHasComboInput = false;
	}

	// Effect
	{
		ChargeParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("UParticleSystemComponent"));
		ChargeParticleSystemComponent->SetupAttachment(RootComponent);
		ChargeParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		static ConstructorHelpers::FObjectFinder<UParticleSystem>ChargeParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura'"));
		if (ChargeParticleRef.Object)
		{
			ChargeParticleSystemComponent->Template = ChargeParticleRef.Object;
		}
	}
}

void AMMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//ChangeClass(EClassType::CT_Beginner);

	// TEST
	{
		ChangeClass(EClassType::CT_Mage);

		if (GetWorld())
		{
			CurrentWeapon = Cast<AMMWeapon>(GetWorld()->SpawnActor<AMMWeapon>(WeaponClass));
			if (CurrentWeapon)
			{
				UE_LOG(LogTemp, Warning, TEXT("Weapon Spawned"));
				EquipWeapon(CurrentWeapon);
			}
		}
	}

	ChargeParticleSystemComponent->SetActive(false);
}

void AMMPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsHold)
	{
		// Spring Arm 길이 조정
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 100.0f, DeltaSeconds, 2.0f);

		// Character Rotation 조정
		FRotator StartRot = GetActorRotation();
		FRotator TargetRot = FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetActorRotation().Roll);
		SetActorRotation(FMath::RInterpTo(StartRot, TargetRot, DeltaSeconds, 4.0f));

		// Camera Position 조정
		Camera->SetRelativeLocation(FVector(0.0f, FMath::FInterpTo(Camera->GetRelativeLocation().Y, 50.0f, DeltaSeconds, 3.0f), FMath::FInterpTo(Camera->GetRelativeLocation().Z, 100.0f, DeltaSeconds, 3.0f)));
	}

	if (bIsCharge)
	{
		ChargeNum = FMath::Clamp(ChargeNum + (DeltaSeconds * 0.3f), 1.0f, 2.0f);
		UE_LOG(LogTemp, Warning, TEXT("%f"), ChargeNum);
	}
}

void AMMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Basic
	EnhancedInputComponent->BindAction(IA_BasicLook, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::BasicLook);
	EnhancedInputComponent->BindAction(IA_BasicMove, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::BasicMove);
	EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::BasicAttack);
	
	// Common
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::DashStart);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &AMMPlayerCharacter::DashEnd);
	EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::RollStart);
	EnhancedInputComponent->BindAction(IA_ConvertWeapon, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertWeapon);

	// Warrior
	EnhancedInputComponent->BindAction(IA_WarriorGuard, ETriggerEvent::Started, this, &AMMPlayerCharacter::GuardStart);
	EnhancedInputComponent->BindAction(IA_WarriorGuard, ETriggerEvent::Completed, this, &AMMPlayerCharacter::GuardEnd);

	// Archer
	EnhancedInputComponent->BindAction(IA_ArcherDraw, ETriggerEvent::Started, this, &AMMPlayerCharacter::DrawArrow);
	EnhancedInputComponent->BindAction(IA_ArcherDraw, ETriggerEvent::Completed, this, &AMMPlayerCharacter::ReleaseArrow);

	// Mage
	EnhancedInputComponent->BindAction(IA_MageSave, ETriggerEvent::Started, this, &AMMPlayerCharacter::SaveStart);
	EnhancedInputComponent->BindAction(IA_MageSave, ETriggerEvent::Completed, this, &AMMPlayerCharacter::SaveEnd);
}

void AMMPlayerCharacter::DashStart()
{
	if (bIsHold) return;

	bIsDash = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AMMPlayerCharacter::DashEnd()
{
	if (bIsHold) return;

	bIsDash = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMMPlayerCharacter::RollStart()
{
	if (bIsRoll) return;
	if (bIsAttacking) return;
	if (bIsHold) return;

	// 애님 인스턴스 가져오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// Roll Check
		bIsRoll = true;

		// 몽타주 재생
		AnimInstance->Montage_Play(RollMontage, 1.3f);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMMPlayerCharacter::RollEnd);

		// RollMontage 종료 시 EndDelegate에 연동된 함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);
	}
}

void AMMPlayerCharacter::RollEnd(class UAnimMontage* Montage, bool IsEnded)
{
	// Roll UnCheck
	bIsRoll = false;
}

void AMMPlayerCharacter::BasicMove(const FInputActionValue& Value)
{
	// 입력받은 Value로부터 MovementVector 가져오기
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 컨트롤러의 회전 중 Yaw(Z)를 가져와 저장
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 회전(Yaw)을 기반으로 전방 및 오른쪽 방향을 받아오기 (X : 전방, Y : 오른쪽)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Movement에 값 전달 (방향, 이동량)
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMMPlayerCharacter::BasicLook(const FInputActionValue& Value)
{
	// 입력받은 Value로부터 LookVector 가져오기
	FVector2D LookVector = Value.Get<FVector2D>();
	
	// Controller에 값 전달
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AMMPlayerCharacter::BasicAttack()
{
	// 구르기 상태일 때 공격 불가
	if (bIsRoll) return;
	// 무기 스왑중에는 공격 불가
	if (bIsChange) return;
	// 초보자가 아닌 직군은 무기를 장착하지 않으면 공격 불가
	if (ClassType != EClassType::CT_Beginner && !bIsEquip) return;
	// 화살 장전 중에는 화살 발사 애니메이션 재생 후 종료
	if (bIsHold)
	{
		if (bCanShoot)
		{
			bCanShoot = false;
			ShootArrow();
		}
		return;
	}

	// 콤보 시작
	if (CurrentComboCount == 0)
	{
		ComboStart();
		bIsAttacking = true;
		return;
	}

	// 중간 입력 체크
	// * 콤보 타이머가 종료되지 않은 상태라면 콤보 입력 체크
	if (ComboTimerHandle.IsValid())
	{
		bHasComboInput = true;
	}
	// * 콤보 타이머가 유효하지 않은(종료) 상태라면 콤보 입력 체크 해제
	else
	{
		bHasComboInput = false;
	}
}

void AMMPlayerCharacter::ComboStart()
{
	// 현재 콤보 수 1로 증가
	CurrentComboCount = 1;

	// 공격 시 플레이어 이동 불가
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// TODO : 공격 속도가 추가되면 값 가져와 지정하기
	const float AttackSpeedRate = 1.0f;


	// 애님 인스턴스 가져오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 몽타주 재생
		AnimInstance->Montage_Play(ComboMontage[ClassType], AttackSpeedRate);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMMPlayerCharacter::ComboEnd);

		// ComboMontage가 종료되면 EndDelegate에 연동된 ComboEnd함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboMontage[ClassType]);

		// 타이머 초기화
		ComboTimerHandle.Invalidate();
		// 타이머 설정
		SetComboTimer();
	}
}

void AMMPlayerCharacter::ComboEnd(UAnimMontage* Montage, bool IsEnded)
{
	// 콤보 수 초기화
	CurrentComboCount = 0;

	// 콤보 입력 판별 초기화
	bHasComboInput = false;
	
	// 공격 종료
	bIsAttacking = false;

	// 플레이어 이동 가능하도록 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AMMPlayerCharacter::ComboCheck()
{
	// 타이머 핸들 초기화
	ComboTimerHandle.Invalidate();

	// 콤보에 대한 입력이 들어온 상황이라면?
	if (bHasComboInput)
	{
		// 콤보 수 증가
		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, ComboData[ClassType]->MaxComboCount);
		
		// 애님 인스턴스 가져오기
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 다음 섹션의 이름 만들기
			FName SectionName = *FString::Printf(TEXT("%s%d"), *ComboData[ClassType]->SectionPrefix, CurrentComboCount);

			// 다음 섹션으로 이동하기
			AnimInstance->Montage_JumpToSection(SectionName, ComboMontage[ClassType]);

			// 타이머 재설정
			SetComboTimer();
			// 콤보 입력 판별 초기화
			bHasComboInput = false;
		}
	}
}

void AMMPlayerCharacter::SetComboTimer()
{
	// 인덱스 조정
	// * 콤보 인덱스 : 1, 2, 3, 4
	// * 배열 인덱스 : 0, 1, 2, 3
	int32 ComboIndex = CurrentComboCount - 1;

	// 인덱스가 유효한지 체크
	if (ComboData[ClassType]->ComboFrame.IsValidIndex(ComboIndex))
	{
		// TODO : 공격 속도가 추가되면 값 가져와 지정하기
		const float AttackSpeedRate = 1.0f;

		// 실제 콤보가 입력될 수 있는 시간 구하기
		float ComboAvailableTime = (ComboData[ClassType]->ComboFrame[ComboIndex] / ComboData[ClassType]->FrameRate) / AttackSpeedRate;

		// 타이머 설정하기
		if (ComboAvailableTime > 0.0f)
		{
			// ComboAvailableTime시간이 지나면 ComboCheck() 함수 호출
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMMPlayerCharacter::ComboCheck, ComboAvailableTime, false);
		}
	}
}

void AMMPlayerCharacter::BaseAttackCheck()
{
	// 충돌 결과를 반환하기 위한 배열
	TArray<FHitResult> OutHitResults;

	// 공격 반경
	float AttackRange = 100.0f;
	// 공격 체크를 위한 구체의 반지름
	float AttackRadius = 50.0f;

	// 충돌 탐지를 위한 시작 지점 (플레이어 현재 위치 + 전방 방향 플레이어의 CapsuleComponent의 반지름 거리)
	FVector Start = GetActorLocation() + (GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius());
	// 충돌 탐지 종료 지점 (시작지점 + 전방 방향의 공격 거리)
	FVector End = Start + (GetActorForwardVector() * AttackRange);
	// 파라미터 설정하기 (트레이스 태그 : Attack, 복잡한 충돌 처리 : false, 무시할 액터 : this) 
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	bool bHasHit = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHasHit)
	{
		// TODO : 데미지 전달
		for (FHitResult Result : OutHitResults)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.GetActor()->GetName());
		}
	}

	// Capsule 모양의 디버깅 체크
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
}

void AMMPlayerCharacter::ChangeClass(EClassType Class)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 기존에 저장된 IMC 초기화
			SubSystem->ClearAllMappings();

			// 새로운 매핑 컨텍스트 연동
			UInputMappingContext* NewMappingContext = IMC_Array[Class];
			SubSystem->AddMappingContext(NewMappingContext, 0);
		}

		ClassType = Class;
	}
}

void AMMPlayerCharacter::ConvertWeapon()
{
	if (bIsChange) return;
	if (bIsHold) return;

	if (bIsEquip)
	{
		SheatheWeapon();
	}
	else
	{
		DrawWeapon();
	}
}

void AMMPlayerCharacter::DrawWeapon()
{
	if (CurrentWeapon)
	{
		bIsChange = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 몽타주 재생
			AnimInstance->Montage_Play(DrawMontage[ClassType]);

			// 몽타주 재생 종료 바인딩
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AMMPlayerCharacter::DrawEnd);

			// DrawMontage가 종료되면 EndDelegate에 연동된 DrawEnd함수 호출
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DrawMontage[ClassType]);
		}
	}
}

void AMMPlayerCharacter::GuardStart()
{
	if (!bIsEquip) return;
	if (bIsAttacking) return;

	// 방어시 플레이어 이동 불가
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	bIsGuard = true;
}

void AMMPlayerCharacter::GuardEnd()
{
	if (!bIsEquip) return;
	if (bIsAttacking) return;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bIsGuard = false;
}

void AMMPlayerCharacter::DrawArrow()
{
	// 장전 중인 경우 반환
	if (bIsHold) return;
	// 구르기중인 경우 반환
	if (bIsRoll) return;
	// 무기 스왑중인 경우 반환
	if (bIsChange) return;
	// 무기를 장착하지 않은 경우 반환
	if (!bIsEquip) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	if (AnimInstance->Montage_IsPlaying(ReleaseArrowMontage) || AnimInstance->Montage_IsPlaying(DrawArrowMontage)) return;

	if (CurrentWeapon)
	{
		// 장전시 플레이어 이동 불가
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		// 플레이어 달리기 취소
		if (bIsDash)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			bIsDash = false;
		}
		
		bIsHold = true;
		bIsStop = false;

		// 움직임 설정
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

		// 몽타주 재생
		AnimInstance->Montage_Play(DrawArrowMontage, 1.0f);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMMPlayerCharacter::DrawArrowEnd);

		// DrawArrowMontage가 종료되면 EndDelegate에 연동된 DrawArrowEnd함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DrawArrowMontage);
	}
}

void AMMPlayerCharacter::ReleaseArrow()
{
	if (!bIsHold) return;
	// 무기를 장착하지 않은 경우 반환
	if (!bIsEquip) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	if (AnimInstance->Montage_IsPlaying(DrawArrowMontage))
	{
		AnimInstance->Montage_Stop(0.1f, DrawArrowMontage);
	}

	if (AnimInstance->Montage_IsPlaying(ReleaseArrowMontage))
	{
		AnimInstance->Montage_Stop(0.1f, ReleaseArrowMontage);
	}

	AMMBowWeapon* BowWeapon = Cast<AMMBowWeapon>(CurrentWeapon);
	if (BowWeapon)
	{
		BowWeapon->SetIsHold(false);
		BowWeapon->DestroyArrow();
	}

	bIsHold = false;
	bCanShoot = false;
	bIsStop = true;

	// 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// 카메라 설정
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	SpringArm->TargetArmLength = 500.0f;
}

void AMMPlayerCharacter::SaveStart()
{
	if (!bIsEquip) return;
	if (bIsAttacking) return;

	// 플레이어 이동 불가
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FTransform ParticleTransform;
	ParticleTransform.SetLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 90.0f));
	ChargeParticleSystemComponent->SetActive(true);

	bIsCharge = true;
}

void AMMPlayerCharacter::SaveEnd()
{
	if (!bIsEquip) return;
	if (bIsAttacking) return;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	ChargeParticleSystemComponent->SetActive(false);
	bIsCharge = false;
}

void AMMPlayerCharacter::DrawEnd(UAnimMontage* Montage, bool IsEnded)
{
	bIsChange = false;
	bIsEquip = true;
}

void AMMPlayerCharacter::SheatheWeapon()
{
	if (CurrentWeapon)
	{
		bIsChange = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 몽타주 재생
			AnimInstance->Montage_Play(SheatheMontage[ClassType]);

			// 몽타주 재생 종료 바인딩
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AMMPlayerCharacter::SheatheEnd);

			// SheatheMontage가 종료되면 EndDelegate에 연동된 SheatheEnd함수 호출
			AnimInstance->Montage_SetEndDelegate(EndDelegate, SheatheMontage[ClassType]);
		}
	}
}

void AMMPlayerCharacter::SheatheEnd(UAnimMontage* Montage, bool IsEnded)
{
	bIsChange = false;
	bIsEquip = false;
}

void AMMPlayerCharacter::EquipWeapon(AMMWeapon* Weapon)
{
	if (!CurrentWeapon) return;

	CurrentWeapon->SetOwner(this);
	Weapon->EquipWeapon();
}

void AMMPlayerCharacter::DrawArrowEnd(UAnimMontage* Montage, bool IsEnded)
{
	// 움직임 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bCanShoot = true;
}

void AMMPlayerCharacter::ReleaseArrowEnd(UAnimMontage* Montage, bool IsEnded)
{
	if (bIsStop) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 몽타주 재생
	AnimInstance->Montage_Play(DrawArrowMontage, 1.5f);

	// 몽타주 재생 종료 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMMPlayerCharacter::DrawArrowEnd);

	// DrawArrowMontage가 종료되면 EndDelegate에 연동된 DrawArrowEnd함수 호출
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DrawArrowMontage);
}

void AMMPlayerCharacter::ShootArrow()
{
	if (!bIsHold) return;
	if (CurrentWeapon)
	{
		// 공격 시 플레이어 이동 불가
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 몽타주 재생
			AnimInstance->Montage_Play(ReleaseArrowMontage);

			// 몽타주 재생 종료 바인딩
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AMMPlayerCharacter::ReleaseArrowEnd);

			// ReleaseArrowMontage가 종료되면 EndDelegate에 연동된 ReleaseArrowEnd함수 호출
			AnimInstance->Montage_SetEndDelegate(EndDelegate, ReleaseArrowMontage);
		}

		// 화살을 발사합니다.
		AMMBowWeapon* BowWeapon = Cast<AMMBowWeapon>(CurrentWeapon);
		if (BowWeapon)
		{
			BowWeapon->ShootArrow();
		}
	}
}
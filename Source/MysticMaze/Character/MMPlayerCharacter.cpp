// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MMPlayerCharacter.h"
#include "MMComboActionData.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"

AMMPlayerCharacter::AMMPlayerCharacter()
{
	// Collision 설정
	{
		GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);

		// 프리셋 지정
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("MMCapsule"));
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

		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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

		// Basic Input
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_BasicRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_BasicPlayer.IMC_BasicPlayer'"));
		if (IMC_BasicRef.Object)
		{
			IMC_Basic = IMC_BasicRef.Object;
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
		GetCharacterMovement()->MaxWalkSpeed = 230.0f;
	}

	// Combo Variable 초기화
	{
		CurrentComboCount = 0;
		bHasComboInput = false;
	}

	// Member Variable 초기화
	{
		bIsDash = false;
		bIsRoll = false;
		bIsAttacking = false;
		WalkSpeed = 230.0f;
		RunSpeed = 600.0f;
	}
}

void AMMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && IMC_Basic)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 매핑 컨텍스트 연동
			SubSystem->AddMappingContext(IMC_Basic, 0);
			// 입력 시작
			EnableInput(PlayerController);
		}
	}
}

void AMMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_BasicLook, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::BasicLook);
	EnhancedInputComponent->BindAction(IA_BasicMove, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::BasicMove);
	EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::BasicAttack);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::DashStart);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &AMMPlayerCharacter::DashEnd);
	EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::RollStart);
}

void AMMPlayerCharacter::DashStart()
{
	bIsDash = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AMMPlayerCharacter::DashEnd()
{
	bIsDash = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMMPlayerCharacter::RollStart()
{
	if (bIsRoll) return;
	if (bIsAttacking) return;

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
		AnimInstance->Montage_Play(BasicComboMontage, AttackSpeedRate);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMMPlayerCharacter::ComboEnd);

		// BasicComboMontage가 종료되면 EndDelegate에 연동된 ComboEnd함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicComboMontage);

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
		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, BasicComboData->MaxComboCount);
		
		// 애님 인스턴스 가져오기
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 다음 섹션의 이름 만들기
			FName SectionName = *FString::Printf(TEXT("%s%d"), *BasicComboData->SectionPrefix, CurrentComboCount);

			// 다음 섹션으로 이동하기
			AnimInstance->Montage_JumpToSection(SectionName, BasicComboMontage);

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
	if (BasicComboData->ComboFrame.IsValidIndex(ComboIndex))
	{
		// TODO : 공격 속도가 추가되면 값 가져와 지정하기
		const float AttackSpeedRate = 1.0f;

		// 실제 콤보가 입력될 수 있는 시간 구하기
		float ComboAvailableTime = (BasicComboData->ComboFrame[ComboIndex] / BasicComboData->FrameRate) / AttackSpeedRate;

		// 타이머 설정하기
		if (ComboAvailableTime > 0.0f)
		{
			// ComboAvailableTime시간이 지나면 ComboCheck() 함수 호출
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMMPlayerCharacter::ComboCheck, ComboAvailableTime, false);
		}
	}
}

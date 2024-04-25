// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MMPlayerCharacter.h"

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
		static ConstructorHelpers::FObjectFinder<UInputAction>IA_DashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/IA_Dash.IA_Dash'"));
		if (IA_DashRef.Object)
		{
			IA_Dash = IA_DashRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_RollRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/IA_Roll.IA_Roll'"));
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

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_BasicMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/IA_BaseMove.IA_BaseMove'"));
		if (IA_BasicMoveRef.Object)
		{
			IA_BasicMove = IA_BasicMoveRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_BasicLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/IA_BaseLook.IA_BaseLook'"));
		if (IA_BasicLookRef.Object)
		{
			IA_BasicLook = IA_BasicLookRef.Object;
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

	// Member Variable 초기화
	{
		bIsDash = false;
		bIsRoll = false;
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
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::DashStart);
	EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &AMMPlayerCharacter::DashEnd);
	EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::RollStart);
}

void AMMPlayerCharacter::DashStart()
{
	bIsDash = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void AMMPlayerCharacter::DashEnd()
{
	bIsDash = false;
	GetCharacterMovement()->MaxWalkSpeed = 230;
}

void AMMPlayerCharacter::RollStart()
{
	if (bIsRoll) return;

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

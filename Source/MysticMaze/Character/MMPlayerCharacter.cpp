// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MMPlayerCharacter.h"
#include "MMComboActionData.h"
#include "Collision/MMCollision.h"
#include "Item/MMItemBox.h"
#include "Item/MMWeapon.h"
#include "Item/MMSwordWeapon.h"
#include "Item/MMBowWeapon.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMPlayerController.h"
#include "Interface/MMInteractionInterface.h"
#include "Containers/Map.h"
#include "UI/MMHUDWidget.h"
#include "Player/MMStatComponent.h"
#include "Player/MMSkillComponent.h"
#include "Skill/MMSkillBase.h"
#include "UI/MMSettingWidget.h"
#include "Game/MMGameInstance.h"

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
#include "Item/MMItemBox.h"
#include "DrawDebugHelpers.h"

AMMPlayerCharacter::AMMPlayerCharacter()
{
	// Tick 활성화
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
		bIsRide = false;
		WalkSpeed = 230.0f;
		RunSpeed = 600.0f;
		RidingSpeed = 1500.0f;
		AutoSaveTime = 15.0f;
		SumTime = 0.0f;

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

		// Ride
		RideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RidingMesh"));
		RideMesh->SetupAttachment(RootComponent);
		RideMesh->SetVisibility(false);

		RideActorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RidingActorMesh"));
		RideActorMesh->SetupAttachment(RideMesh);
		RideActorMesh->SetVisibility(false);
	}

	// Camera 설정
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->TargetArmLength = 800.0f;

		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
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

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_InteractionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_Interaction.IA_Interaction'"));
		if (IA_InteractionRef.Object)
		{
			IA_Interaction = IA_InteractionRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertInventoryRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertInventory.IA_ConvertInventory'"));
		if (IA_ConvertInventoryRef.Object)
		{
			IA_ConvertInventory = IA_ConvertInventoryRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertStatusRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertStatus.IA_ConvertStatus'"));
		if (IA_ConvertStatusRef.Object)
		{
			IA_ConvertStatus = IA_ConvertStatusRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertEquipmentRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertEquipment.IA_ConvertEquipment'"));
		if (IA_ConvertEquipmentRef.Object)
		{
			IA_ConvertEquipment = IA_ConvertEquipmentRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertSkill.IA_ConvertSkill'"));
		if (IA_ConvertSkillRef.Object)
		{
			IA_ConvertSkill = IA_ConvertSkillRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertRidingRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertRiding.IA_ConvertRiding'"));
		if (IA_ConvertRidingRef.Object)
		{
			IA_ConvertRiding = IA_ConvertRidingRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_ConvertSettingRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_ConvertSetting.IA_ConvertSetting'"));
		if (IA_ConvertSettingRef.Object)
		{
			IA_ConvertSetting = IA_ConvertSettingRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_CheatRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_Cheat.IA_Cheat'"));
		if (IA_CheatRef.Object)
		{
			IA_Cheat = IA_CheatRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_QuickSlot1Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_QuickSlot1.IA_QuickSlot1'"));
		if (IA_QuickSlot1Ref.Object)
		{
			IA_QuickSlot1 = IA_QuickSlot1Ref.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_QuickSlot2Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_QuickSlot2.IA_QuickSlot2'"));
		if (IA_QuickSlot2Ref.Object)
		{
			IA_QuickSlot2 = IA_QuickSlot2Ref.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_QuickSlot3Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_QuickSlot3.IA_QuickSlot3'"));
		if (IA_QuickSlot3Ref.Object)
		{
			IA_QuickSlot3 = IA_QuickSlot3Ref.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_QuickSlot4Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_QuickSlot4.IA_QuickSlot4'"));
		if (IA_QuickSlot4Ref.Object)
		{
			IA_QuickSlot4 = IA_QuickSlot4Ref.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_QuickSlot5Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_QuickSlot5.IA_QuickSlot5'"));
		if (IA_QuickSlot5Ref.Object)
		{
			IA_QuickSlot5 = IA_QuickSlot5Ref.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction>IA_QuickSlot6Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/MysticMaze/Player/Control/InputAction/Common/IA_QuickSlot6.IA_QuickSlot6'"));
		if (IA_QuickSlot6Ref.Object)
		{
			IA_QuickSlot6 = IA_QuickSlot6Ref.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_RidingRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_Riding.IMC_Riding'"));
		if (IMC_RidingRef.Object)
		{
			IMC_Riding = IMC_RidingRef.Object;
		}

		// Basic Input
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_BasicRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MysticMaze/Player/Control/IMC_BasicPlayer.IMC_BasicPlayer'"));
		if (IMC_BasicRef.Object)
		{
			IMC_Array.Add(EClassType::CT_Beginner, IMC_BasicRef.Object);
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

	// etc. Component
	{
		Inventory = CreateDefaultSubobject<UMMInventoryComponent>(TEXT("Inventory"));
		Skill = CreateDefaultSubobject<UMMSkillComponent>(TEXT("Skill"));
	}
}


void AMMPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 델리게이트 연동
	Stat->OnMovementSpeedChanged.AddUObject(this, &AMMPlayerCharacter::ApplyMovementSpeed);
	Stat->OnHpZero.AddUObject(this, &AMMPlayerCharacter::Death);
	Stat->OnHit.AddUObject(this, &AMMPlayerCharacter::Hit);

	// 스탯 컴포넌트 초기화
	Stat->Init();

	// 파티클 비활성화
	ChargeParticleSystemComponent->SetActive(false);
}

void AMMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChangeClass(ClassType);

	// TEST
	//FTransform SpawnTransform;
	//SpawnTransform.SetLocation(GetActorLocation() - GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	//AMMItemBox* ItemBox = GetWorld()->SpawnActorDeferred<AMMItemBox>(AMMItemBox::StaticClass(), SpawnTransform);
	//if (ItemBox)
	//{
	//	ItemBox->AddItemQuantity(50);
	//	ItemBox->AddMoney(1000);
	//	ItemBox->FinishSpawning(SpawnTransform);
	//}
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
		SumTime += DeltaSeconds;

		if (SumTime >= 0.2f)
		{
			// 마나 및 체력 회복
			Stat->HealHp(0.1f);
			Stat->HealMp(0.1f);
		}
	}

	AutoSaveTime -= DeltaSeconds;
	if (AutoSaveTime <= 0.0f)
	{
		AutoSaveTime = 15.0f;
		Save();
	}
}

float AMMPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float ActualDamage = DamageAmount;

	// 구르기 중이면 데미지 받지 않기
	if (bIsRoll) return 0.0f;

	// 가드 중이면 데미지 50% 절감
	if (bIsGuard)
	{
		ActualDamage *= 0.5f;
	}

	// 스킬 사용 중이라면?
	//if (IsValid(Skill->GetUsingSkill()))
	//{
	//	// 스킬 캔슬하기
	//	Skill->GetUsingSkill()->Cancel();
	//}

	// 데미지 적용하기
	Stat->ApplyDamage(ActualDamage);

	return DamageAmount;
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
	EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::Interaction);
	EnhancedInputComponent->BindAction(IA_ConvertInventory, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertInventoryVisibility);
	EnhancedInputComponent->BindAction(IA_ConvertStatus, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertStatusVisibility);
	EnhancedInputComponent->BindAction(IA_ConvertSkill, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertSkillVisibility);
	EnhancedInputComponent->BindAction(IA_ConvertRiding, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertRiding);

	EnhancedInputComponent->BindAction(IA_QuickSlot1, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::UseQuickSlot, 1);
	EnhancedInputComponent->BindAction(IA_QuickSlot2, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::UseQuickSlot, 2);
	EnhancedInputComponent->BindAction(IA_QuickSlot3, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::UseQuickSlot, 3);
	EnhancedInputComponent->BindAction(IA_QuickSlot4, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::UseQuickSlot, 4);
	EnhancedInputComponent->BindAction(IA_QuickSlot5, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::UseQuickSlot, 5);
	EnhancedInputComponent->BindAction(IA_QuickSlot6, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::UseQuickSlot, 6);
	EnhancedInputComponent->BindAction(IA_ConvertEquipment, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertEquipmentVisibility);
	EnhancedInputComponent->BindAction(IA_ConvertSetting, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::ConvertSetting);
	
	EnhancedInputComponent->BindAction(IA_Cheat, ETriggerEvent::Triggered, this, &AMMPlayerCharacter::Cheat);
	
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

void AMMPlayerCharacter::Save()
{
	Inventory->SaveInventory();
	Skill->SaveSkill();
	Stat->SaveStat();

	UMMGameInstance* GM = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GM)
	{
		GM->SaveDungeonFloor();
	}
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
	if (Skill->GetUsingSkill()) return;

	// 애님 인스턴스 가져오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// Roll Check
		bIsRoll = true;

		// 콜리전 충돌 무효
		GetCapsuleComponent()->SetCollisionProfileName(MMTRIGGER);

		// 키보드 방향으로 회전
		{
			// 컨트롤러의 회전 중 Yaw(Z)를 가져와 저장
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// 회전(Yaw)을 기반으로 전방 및 오른쪽 방향을 받아오기 (X : 전방, Y : 오른쪽)
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// 전방 및 오른쪽 방향을 이용하여 목표 회전 각도 계산
			const FRotator TargetRotation = (ForwardDirection * MovementVector.X + RightDirection * MovementVector.Y).Rotation();

			// 컨트롤러를 목표 회전 각도로 설정
			SetActorRotation(TargetRotation);
		}

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

	// 콜리전 충돌 활성화
	GetCapsuleComponent()->SetCollisionProfileName(MMCAPSULE);
}

void AMMPlayerCharacter::ConvertInventoryVisibility()
{
	// 인벤토리 On/Off 설정
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->GetHUDWidget())
		{
			// 인벤토리 위젯 토글 함수를 호출합니다.
			PlayerController->GetHUDWidget()->ToggleInventoryWidget();

			// 현재 활성화된 위젯에 대한 비트플래그를 확인하여 모드를 변경해주도록 합니다.
			if (PlayerController->GetHUDWidget()->GetIsVisibility())
			{
				// 활성화된 위젯이 있으므로 UI모드로 설정합니다.
				PlayerController->SetUIInputMode();
			}
			else
			{
				// 활성화된 위젯이 없으므로 UI모드로 설정합니다.
				PlayerController->SetGameInputMode();
			}
		}
	}
}

void AMMPlayerCharacter::ConvertStatusVisibility()
{
	// 스테이터스 On/Off 설정
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->GetHUDWidget())
		{
			// 스테이터스 위젯 토글 함수를 호출합니다.
			PlayerController->GetHUDWidget()->ToggleStatusWidget();

			// 현재 활성화된 위젯에 대한 비트플래그를 확인하여 모드를 변경해주도록 합니다.
			if (PlayerController->GetHUDWidget()->GetIsVisibility())
			{
				// 활성화된 위젯이 있으므로 UI모드로 설정합니다.
				PlayerController->SetUIInputMode();
			}
			else
			{
				// 활성화된 위젯이 없으므로 UI모드로 설정합니다.
				PlayerController->SetGameInputMode();
			}
		}
	}
}

void AMMPlayerCharacter::ConvertEquipmentVisibility()
{
	// 장비 위젯 On/Off 설정
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->GetHUDWidget())
		{
			// 장비 위젯 토글 함수를 호출합니다.
			PlayerController->GetHUDWidget()->ToggleEquipmentWidget();

			// 현재 활성화된 위젯에 대한 비트플래그를 확인하여 모드를 변경해주도록 합니다.
			if (PlayerController->GetHUDWidget()->GetIsVisibility())
			{
				// 활성화된 위젯이 있으므로 UI모드로 설정합니다.
				PlayerController->SetUIInputMode();
			}
			else
			{
				// 활성화된 위젯이 없으므로 UI모드로 설정합니다.
				PlayerController->SetGameInputMode();
			}
		}
	}
}

void AMMPlayerCharacter::ConvertSkillVisibility()
{
	// 스킬 위젯 On/Off 설정
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->GetHUDWidget())
		{
			// 스킬 위젯 토글 함수를 호출합니다.
			PlayerController->GetHUDWidget()->ToggleSkillWidget();

			// 현재 활성화된 위젯에 대한 비트플래그를 확인하여 모드를 변경해주도록 합니다.
			if (PlayerController->GetHUDWidget()->GetIsVisibility())
			{
				// 활성화된 위젯이 있으므로 UI모드로 설정합니다.
				PlayerController->SetUIInputMode();
			}
			else
			{
				// 활성화된 위젯이 없으므로 UI모드로 설정합니다.
				PlayerController->SetGameInputMode();
			}
		}
	}
}

void AMMPlayerCharacter::ConvertRiding()
{
	// 특정 동작 중에는 탈것 소환 X
	if (bIsRoll) return;
	if (bIsChange) return;
	if (bIsAttacking) return;
	if (bIsGuard) return;
	if (bIsHold) return;
	if (bIsCharge) return;

	// 탈것 활성화 Toggle
	if (!bIsRide) 
		bIsRide = true;
	else 
		bIsRide = false;

	// 탈것이 활성화 되었다면?
	if (bIsRide)
	{
		// 탈것 메시 활성화
		RideMesh->SetVisibility(true);
		RideActorMesh->SetVisibility(true);
		// 기본 메시 비활성화
		GetMesh()->SetVisibility(false);
		// 이동속도 조정 (1000)
		GetCharacterMovement()->MaxWalkSpeed = RidingSpeed;
		// 달리기 상태 초기화
		bIsDash = false;
		// 만약 착용한 무기가 있다면?
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetHidden(true);
		}
	}
	// 탈것이 비활성화 되었다면?
	else
	{
		// 탈것 메시 비활성화
		RideMesh->SetVisibility(false);
		RideActorMesh->SetVisibility(false);
		// 기본 메시 활성화
		GetMesh()->SetVisibility(true);
		// 이동속도 조정
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		// 만약 착용한 무기가 있다면?
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetHidden(false);
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 기존에 저장된 IMC 초기화
			SubSystem->ClearAllMappings();

			if (bIsRide)
			{
				// Riding 매핑 컨텍스트 연동
				UInputMappingContext* NewMappingContext = IMC_Riding;
				SubSystem->AddMappingContext(NewMappingContext, 0);
			}
			else
			{
				// 현재 직업 매핑 컨텍스트 연동
				UInputMappingContext* NewMappingContext = IMC_Array[ClassType];
				SubSystem->AddMappingContext(NewMappingContext, 0);
			}
		}
	}
}

void AMMPlayerCharacter::ConvertSetting()
{
	// 설정 위젯 On/Off 설정
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->GetSettingWidget()->GetVisibility() == ESlateVisibility::Hidden)
		{
			PlayerController->GetSettingWidget()->SetVisibility(ESlateVisibility::Visible);

			// 활성화된 위젯이 있으므로 UI모드로 설정합니다.
			PlayerController->SetUIInputMode();
		}
	}
}

void AMMPlayerCharacter::BasicMove(const FInputActionValue& Value)
{
	// 입력받은 Value로부터 MovementVector 가져오기
	MovementVector = Value.Get<FVector2D>();
	//FVector2D MovementVector = Value.Get<FVector2D>();

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
	// 스킬 사용중에는 반환
	if (Skill->GetUsingSkill()) return;
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
		SetActorRotation(FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetActorRotation().Roll));
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

	// 공격 속도 반영하기
	const float AttackSpeedRate = Stat->GetAttackSpeed();

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
		// 공격속도 지정하기
		const float AttackSpeedRate = Stat->GetAttackSpeed();

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
		// 데미지 전달
		for (FHitResult Result : OutHitResults)
		{
			ACharacter* PlayerCharacter = Cast<ACharacter>(Result.GetActor());
			if (PlayerCharacter)
			{
				float Damage = Stat->GetAttackDamage();
				bool Critical = FMath::FRand() < (Stat->GetCriticalHitRate() / 100);
				
				if (Critical)
				{
					Damage *= 2.0f;
				}

				UGameplayStatics::ApplyDamage(Result.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
			}
		}
	}
}

void AMMPlayerCharacter::Hit()
{
	// 탈것이 활성화 되어 있는 상황이라면?
	if (bIsRide)
	{
		// 탈것 해제하기
		ConvertRiding();
	}

	if (!bIsGuard && !Skill->GetUsingSkill())
	{
		// Hit 몽타주 재생
		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	}
}

void AMMPlayerCharacter::Death()
{
	// 애님 인스턴스 가져오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// Death 몽타주 재생
		AnimInstance->Montage_Play(DeathMontage);

		// 몽타주 재생 종료 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMMPlayerCharacter::DeathEnd);

		// DeathMontage가 종료되면 EndDelegate에 연동된 DeathEnd함수 호출
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathMontage);
	}

	// 캐릭터 이동 기능 제한
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 리스폰 연동
	RespawnTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AMMPlayerCharacter::Respawn, 3.0f, false);
}

void AMMPlayerCharacter::DeathEnd(UAnimMontage* Montage, bool IsEnded)
{
	// 캐릭터 안보이게 설정
	GetMesh()->SetVisibility(false);
	// 플레이어 충돌제거
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMMPlayerCharacter::Respawn()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TownLevel"));
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
	if (bIsRoll) return;
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
	// 스킬 사용중이라면 반환
	if (Skill->GetUsingSkill()) return;

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

		// 스테이터스바 위젯 감추기
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
		if (PlayerController)
		{
			if (PlayerController->GetHUDWidget())
			{
				// 스테이터스바 위젯 토글 함수를 호출합니다.
				PlayerController->GetHUDWidget()->ToggleStatusBarWidget();
			}
		}

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

	// 스테이터스바 위젯 출력
	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->GetHUDWidget())
		{
			// 스테이터스바 위젯 토글 함수를 호출합니다.
			PlayerController->GetHUDWidget()->ToggleStatusBarWidget();
		}
	}

	// 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// 카메라 설정
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	SpringArm->TargetArmLength = 800.0f;
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

	if (CurrentWeapon)
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
	if (!Weapon) return;

	// 기존 무기가 있다면 장착 해제하기
	if (CurrentWeapon)
	{
		UnEquipWeapon();
	}

	// 현재 무기를 업데이트 및 착용
	CurrentWeapon = Weapon;
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->EquipWeapon();

	bIsEquip = false;
	bIsChange = false;
}

void AMMPlayerCharacter::UnEquipWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
		bIsEquip = false;
		bIsChange = false;
	}
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

void AMMPlayerCharacter::ApplyMovementSpeed(float MovementSpeed)
{
	// 이동속도 설정
	float AdditiveSpeed = MovementSpeed - 600;

	// 이동속도 적용
	if (bIsDash)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed + AdditiveSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed + AdditiveSpeed;
	}

	UE_LOG(LogTemp, Warning, TEXT("SetSpeed, %f, %f"), WalkSpeed, RunSpeed);
}

void AMMPlayerCharacter::Interaction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	if (AnimInstance->Montage_IsPlaying(PickUpMontage)) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams;

	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		CHANNEL_MMINTERACTION,
		FCollisionShape::MakeSphere(100.0f),
		CollisionQueryParams
	);

	if (bHasHit)
	{
		// 충돌한 물체들을 순회하며 상호작용 합니다.
		for (const FOverlapResult& Result : OverlapResults)
		{
			IMMInteractionInterface* InteractionActor = Cast<IMMInteractionInterface>(Result.GetActor());

			// 상호작용이 가능한 액터라면?
			if (InteractionActor)
			{
				// 상호작용 기능을 수행합니다.
				InteractionActor->Interaction(this);
				break;
			}
		}
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), 100.0f, 16, FColor::Green, false, 1.0f);
}

void AMMPlayerCharacter::UseQuickSlot(int32 InNum)
{
	switch (InNum)
	{
	// Skill QuickSlot
	case 1:
	case 2:
	case 3:
	case 4:
		if (bIsEquip)
			Skill->UseSkill(InNum - 1);
		break;

	// Potion QuickSlot
	case 5:
	case 6:
		Inventory->UseItem(InNum - 5, ESlotType::ST_PotionSlot);
		break;
	}
}

void AMMPlayerCharacter::Cheat()
{
	// 경험치 증가
	Stat->SetExp(1000.0f);

	// 골드 증가
	Inventory->AddGold(100);
	
	// 마석 얻기
	int32 Temp = 0;
	Inventory->AddItem(TEXT("DA_ManaStone"), 3, Temp);
}

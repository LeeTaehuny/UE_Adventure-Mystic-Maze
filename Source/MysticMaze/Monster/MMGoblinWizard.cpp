// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGoblinWizard.h"

#include "MonsterAI/AIController/MMGoblinWizardAIController.h"

#include "Components/CapsuleComponent.h"
#include "Collision/MMCollision.h"
#include "Monster/Magic/MMFireBall.h"
#include "Components/SceneComponent.h"

AMMGoblinWizard::AMMGoblinWizard()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
		ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Goblin/Wizard/Animations/ABP_GoblinWizard.ABP_GoblinWizard_C'"));
		if (SkeletalMeshRef.Object)
		{
			GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Wizard/SK_Wizard_Hat.SK_Wizard_Hat'"));
		if (HeadMeshRef.Object)
		{
			HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
			HeadMesh->SetSkeletalMesh(HeadMeshRef.Object);
			HeadMesh->SetupAttachment(GetMesh());
			HeadMesh->SetCollisionProfileName(TEXT("NoCollision"));

			HeadMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				HeadMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyArmorMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Wizard/SK_Wizard_Cloak.SK_Wizard_Cloak'"));
		if (BodyArmorMeshRef.Object)
		{
			BodyArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyArmor Mesh"));
			BodyArmorMesh->SetSkeletalMesh(BodyArmorMeshRef.Object);
			BodyArmorMesh->SetupAttachment(GetMesh());
			BodyArmorMesh->SetCollisionProfileName(TEXT("NoCollision"));

			BodyArmorMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				BodyArmorMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> BracersMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Wizard/SK_Wizard_Bracers.SK_Wizard_Bracers'"));
		if (BracersMeshRef.Object)
		{
			BracersMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bracers Mesh"));
			BracersMesh->SetSkeletalMesh(BracersMeshRef.Object);
			BracersMesh->SetupAttachment(GetMesh());
			BracersMesh->SetCollisionProfileName(TEXT("NoCollision"));

			BracersMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				BracersMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> PantsMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Wizard/SK_Wizard_Pants.SK_Wizard_Pants'"));
		if (PantsMeshRef.Object)
		{
			PantsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants Mesh"));
			PantsMesh->SetSkeletalMesh(PantsMeshRef.Object);
			PantsMesh->SetupAttachment(GetMesh());
			PantsMesh->SetCollisionProfileName(TEXT("NoCollision"));

			PantsMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				PantsMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottleMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Wizard/SK_Wizard_Bottle.SK_Wizard_Bottle'"));
		if (BottleMeshRef.Object)
		{
			BottleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottle Mesh"));
			BottleMesh->SetSkeletalMesh(BottleMeshRef.Object);
			BottleMesh->SetupAttachment(GetMesh());
			BottleMesh->SetCollisionProfileName(TEXT("NoCollision"));

			BottleMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				BottleMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaffMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Weapons/SK_Goblin_Wizard_Staff.SK_Goblin_Wizard_Staff'"));
		ConstructorHelpers::FClassFinder<UAnimInstance> WeaponAnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Goblin/Wizard/Animations/AMP_GoblinWizardWeapon.AMP_GoblinWizardWeapon_C'"));
		if (StaffMeshRef.Object)
		{
			StaffMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Staff Mesh"));
			StaffMesh->SetSkeletalMesh(StaffMeshRef.Object);
			StaffMesh->SetupAttachment(GetMesh());
			StaffMesh->SetCollisionProfileName(TEXT("NoCollision"));

			StaffMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (WeaponAnimInstanceClassRef.Class)
			{
				StaffMesh->SetAnimInstanceClass(WeaponAnimInstanceClassRef.Class);
			}
		}
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinWizardAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/Wizard/AI/BP_GoblinWizardAIController.BP_GoblinWizardAIController_C'"));
	if (BehaviorRef.Succeeded())
	{
		this->AIControllerClass = BehaviorRef.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}

	// 캐릭터의 걷는 속도 설정
	//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ATK_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Rush Check"));
	ATK_Collision->SetupAttachment(GetMesh());
	ATK_Collision->SetCollisionProfileName(TEXT("NoCollision"));

	bRandomDieMotion = FMath::RandRange(0, 1);

	// 전투 관련 변수 선언
	{
		NormalATKSign = false;
	}
}

void AMMGoblinWizard::BeginPlay()
{
	Super::BeginPlay();

	ATK_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterBase::ATKBeginOverlap);
	ATK_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterBase::ATKEndOverlap);
}

void AMMGoblinWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMMGoblinWizard::MonsterDieMontage()
{
	MontageAutoPlay(DieMontage, Die2Montage);

	int RandomATKMotionStart = FMath::RandRange(1, 2);
	switch (RandomATKMotionStart)
	{
	case 1:
		MontageAutoSection(DieMontage, Die2Montage, "Die1");
		break;

	case 2:
		MontageAutoSection(DieMontage, Die2Montage, "Die2");
		break;
	}
}

void AMMGoblinWizard::MontageAutoPlay(UAnimMontage* INBodyData, UAnimMontage* INWeaponData)
{
	GetMesh()->GetAnimInstance()->Montage_Play(INBodyData);
	HeadMesh->GetAnimInstance()->Montage_Play(INBodyData);
	BodyArmorMesh->GetAnimInstance()->Montage_Play(INBodyData);
	BracersMesh->GetAnimInstance()->Montage_Play(INBodyData);
	PantsMesh->GetAnimInstance()->Montage_Play(INBodyData);
	BottleMesh->GetAnimInstance()->Montage_Play(INBodyData);

	StaffMesh->GetAnimInstance()->Montage_Play(INWeaponData);
}
void AMMGoblinWizard::MontageAutoSection(UAnimMontage* INBodyData, UAnimMontage* INWeaponData, FName INData)
{
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(INData, INBodyData);
	HeadMesh->GetAnimInstance()->Montage_JumpToSection(INData, INBodyData);
	BodyArmorMesh->GetAnimInstance()->Montage_JumpToSection(INData, INBodyData);
	BracersMesh->GetAnimInstance()->Montage_JumpToSection(INData, INBodyData);
	PantsMesh->GetAnimInstance()->Montage_JumpToSection(INData, INBodyData);
	BottleMesh->GetAnimInstance()->Montage_JumpToSection(INData, INBodyData);

	StaffMesh->GetAnimInstance()->Montage_JumpToSection(INData, INWeaponData);
}

void AMMGoblinWizard::ATKChecking()
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
		Params);

	if (bHasHit)
	{
		// TODO : 데미지 전달
		for (FHitResult Result : OutHitResults)
		{
			if (AMMMonsterBase* Monster = Cast<AMMMonsterBase>(Result.GetActor()))
			{
				continue;
			}

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.GetActor()->GetName());

			UGameplayStatics::ApplyDamage(Result.GetActor(),
				100.0f, GetController(),
				this,
				UDamageType::StaticClass());
		}
	}

	// Capsule 모양의 디버깅 체크
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
}

void AMMGoblinWizard::PlayingHowl()
{
	MontageAutoPlay(BodyHowl, WeaponHowl);
}

void AMMGoblinWizard::Monsterdie()
{
	this->Destroy();
}

void AMMGoblinWizard::StartATK()
{
	FTransform Transform;
	Transform.SetLocation(this->GetActorLocation());
	Transform.SetScale3D(FVector(0.5f));

	AMMFireBall* Vall = GetWorld()->SpawnActor<AMMFireBall>(FireBallBPData, Transform);

	if (Vall)
	{
		Vall->SetDirectionLocation(this->GetActorLocation());
	}
}

void AMMGoblinWizard::StartATKMonatage(int INData)
{
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(BodyNormalATK))
	{
		return;
	}

	MontageAutoPlay(BodyNormalATK, WeaponNormalATK);

	if (INData == 1)
	{
		MontageAutoSection(BodyNormalATK, WeaponNormalATK, "ATK1");
	}
	else if (INData == 2)
	{
		MontageAutoSection(BodyNormalATK, WeaponNormalATK, "ATK3");
	}	
}

bool AMMGoblinWizard::GetMontagePlaying()
{
	return GetMesh()->GetAnimInstance()->Montage_IsPlaying(BodyNormalATK);
}

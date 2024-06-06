// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGoblinWarrior.h"

#include "MonsterAI/AIController/MMGoblinWarriorAIController.h"

#include "Components/CapsuleComponent.h"
#include "Collision/MMCollision.h"

AMMGoblinWarrior::AMMGoblinWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		// 스켈레톤 몸통 할당
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
		ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Goblin/Warrior/Animations/ABP_GoblinWarrior.ABP_GoblinWarrior_C'"));
		if (SkeletalMeshRef.Object)
		{
			GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}

			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		}

		// 고블린의 빵탄
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_armor/SK_Head_Helmet.SK_Head_Helmet'"));
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

		// 고블린의 몸통 갑옷
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyArmorMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_armor/SK_Goblin_Body_Armor.SK_Goblin_Body_Armor'"));
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

		// 고블린의 손목 방어구
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> BracersMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_armor/SK_Goblin_Bracers.SK_Goblin_Bracers'"));
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

		// 고블린의 치마..?
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkirtMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_armor/SK_Goblin_skirt.SK_Goblin_skirt'"));
		if (SkirtMeshRef.Object)
		{
			SkirtMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skirt Mesh"));
			SkirtMesh->SetSkeletalMesh(SkirtMeshRef.Object);
			SkirtMesh->SetupAttachment(GetMesh());
			SkirtMesh->SetCollisionProfileName(TEXT("NoCollision"));

			SkirtMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				SkirtMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		// 고블린의 다리 방어구
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> LegArmorMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_armor/SK_Goblin_Leg_Armor.SK_Goblin_Leg_Armor'"));
		if (LegArmorMeshRef.Object)
		{
			LegArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegArmor Mesh"));
			LegArmorMesh->SetSkeletalMesh(LegArmorMeshRef.Object);
			LegArmorMesh->SetupAttachment(GetMesh());
			LegArmorMesh->SetCollisionProfileName(TEXT("NoCollision"));

			LegArmorMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (AnimInstanceClassRef.Class)
			{
				LegArmorMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
			}
		}

		// 고블린의 검
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SwordMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Weapons/SK_Sword.SK_Sword'"));
		ConstructorHelpers::FClassFinder<UAnimInstance> WeaponAnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Goblin/Warrior/Animations/ABP_GoblinWarrioWeapon.ABP_GoblinWarrioWeapon_C'"));
		if (SwordMeshRef.Object)
		{
			SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword Mesh"));
			SwordMesh->SetSkeletalMesh(SwordMeshRef.Object);
			SwordMesh->SetupAttachment(GetMesh());
			SwordMesh->SetCollisionProfileName(TEXT("NoCollision"));

			SwordMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (WeaponAnimInstanceClassRef.Class)
			{
				SwordMesh->SetAnimInstanceClass(WeaponAnimInstanceClassRef.Class);
			}
		}

		// 고블린의 방패
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShieldMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Weapons/SK_Shield_A.SK_Shield_A'"));
		if (ShieldMeshRef.Object)
		{
			ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield Mesh"));
			ShieldMesh->SetSkeletalMesh(ShieldMeshRef.Object);
			ShieldMesh->SetupAttachment(GetMesh());
			ShieldMesh->SetCollisionProfileName(TEXT("NoCollision"));

			ShieldMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			if (WeaponAnimInstanceClassRef.Class)
			{
				ShieldMesh->SetAnimInstanceClass(WeaponAnimInstanceClassRef.Class);
			}
		}
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinWarriorAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/Warrior/AI/BP_GoblinWarriorAIController.BP_GoblinWarriorAIController_C'"));
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
}

void AMMGoblinWarrior::BeginPlay()
{
	Super::BeginPlay();

	ATK_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMMMonsterBase::ATKBeginOverlap);
	ATK_Collision->OnComponentEndOverlap.AddDynamic(this, &AMMMonsterBase::ATKEndOverlap);
}

void AMMGoblinWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMMGoblinWarrior::MonsterDieMontage()
{
	MontageAutoPlay(DieMontage, Die2Montage);

	int RandomATKMotionStart = FMath::RandRange(1, 2);
	switch (RandomATKMotionStart)
	{
	case 1:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);
		HeadMesh->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);
		BodyArmorMesh->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);
		BracersMesh->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);
		SkirtMesh->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);
		LegArmorMesh->GetAnimInstance()->Montage_JumpToSection("Die1", DieMontage);

		SwordMesh->GetAnimInstance()->Montage_JumpToSection("Die1", Die2Montage);
		ShieldMesh->GetAnimInstance()->Montage_JumpToSection("Die1", Die2Montage);
		break;

	case 2:
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);
		HeadMesh->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);
		BodyArmorMesh->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);
		BracersMesh->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);
		SkirtMesh->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);
		LegArmorMesh->GetAnimInstance()->Montage_JumpToSection("Die2", DieMontage);

		SwordMesh->GetAnimInstance()->Montage_JumpToSection("Die2", Die2Montage);
		ShieldMesh->GetAnimInstance()->Montage_JumpToSection("Die2", Die2Montage);
		break;
	}
}

void AMMGoblinWarrior::MontageAutoPlay(UAnimMontage* INBodyData, UAnimMontage* INWeaponData)
{
	GetMesh()->GetAnimInstance()->Montage_Play(INBodyData);
	HeadMesh->GetAnimInstance()->Montage_Play(INBodyData);
	BodyArmorMesh->GetAnimInstance()->Montage_Play(INBodyData);
	BracersMesh->GetAnimInstance()->Montage_Play(INBodyData);
	SkirtMesh->GetAnimInstance()->Montage_Play(INBodyData);
	LegArmorMesh->GetAnimInstance()->Montage_Play(INBodyData);

	SwordMesh->GetAnimInstance()->Montage_Play(INWeaponData);
	ShieldMesh->GetAnimInstance()->Montage_Play(INWeaponData);
}

void AMMGoblinWarrior::ATKChecking()
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

void AMMGoblinWarrior::PlayingHowl()
{
	MontageAutoPlay(BodyHowl, WeaponHowl);
}

void AMMGoblinWarrior::WarriorNormalATK()
{
	if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		MontageAutoPlay(BodyATK, WeaponATK);

		int RandomATKMotionStart = FMath::RandRange(1, 2);
		switch (RandomATKMotionStart)
		{
		case 1:
			GetMesh()->GetAnimInstance()->Montage_JumpToSection("Default", BodyATK);
			HeadMesh->GetAnimInstance()->Montage_JumpToSection("Default", BodyATK);
			BodyArmorMesh->GetAnimInstance()->Montage_JumpToSection("Default", BodyATK);
			BracersMesh->GetAnimInstance()->Montage_JumpToSection("Default", BodyATK);
			SkirtMesh->GetAnimInstance()->Montage_JumpToSection("Default", BodyATK);
			LegArmorMesh->GetAnimInstance()->Montage_JumpToSection("Default", BodyATK);

			SwordMesh->GetAnimInstance()->Montage_JumpToSection("Default", WeaponATK);
			ShieldMesh->GetAnimInstance()->Montage_JumpToSection("Default", WeaponATK);
			break;

		case 2:
			GetMesh()->GetAnimInstance()->Montage_JumpToSection("Slash", BodyATK);
			HeadMesh->GetAnimInstance()->Montage_JumpToSection("Slash", BodyATK);
			BodyArmorMesh->GetAnimInstance()->Montage_JumpToSection("Slash", BodyATK);
			BracersMesh->GetAnimInstance()->Montage_JumpToSection("Slash", BodyATK);
			SkirtMesh->GetAnimInstance()->Montage_JumpToSection("Slash", BodyATK);
			LegArmorMesh->GetAnimInstance()->Montage_JumpToSection("Slash", BodyATK);

			SwordMesh->GetAnimInstance()->Montage_JumpToSection("Slash", WeaponATK);
			ShieldMesh->GetAnimInstance()->Montage_JumpToSection("Slash", WeaponATK);
			break;
		}
	}	
}

void AMMGoblinWarrior::Monsterdie()
{
	this->Destroy();
}

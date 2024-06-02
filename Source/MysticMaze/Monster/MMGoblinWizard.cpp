// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGoblinWizard.h"

#include "MonsterAI/AIController/MMGoblinWizardAIController.h"

#include "Components/CapsuleComponent.h"
#include "Collision/MMCollision.h"

AMMGoblinWizard::AMMGoblinWizard()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/MysticMaze/Monster/Goblin/Animations/ABP_GoblinWizard.ABP_GoblinWizard_C'"));
		if (AnimInstanceClassRef.Class)
		{
			GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
		}

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FClassFinder<AMMGoblinWizardAIController> BehaviorRef(TEXT("/Script/Engine.Blueprint'/Game/MysticMaze/Monster/Goblin/Wizard/AI/BP_GoblinWizardAIController.BP_GoblinWizardAIController_C'"));
	if (BehaviorRef.Succeeded())
	{
		this->AIControllerClass = BehaviorRef.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}

	// ĳ������ �ȴ� �ӵ� ����
	//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ATK_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Rush Check"));
	ATK_Collision->SetupAttachment(GetMesh());
	ATK_Collision->SetCollisionProfileName(TEXT("NoCollision"));
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

void AMMGoblinWizard::ATKChecking()
{
	// �浹 ����� ��ȯ�ϱ� ���� �迭
	TArray<FHitResult> OutHitResults;

	// ���� �ݰ�
	float AttackRange = 100.0f;
	// ���� üũ�� ���� ��ü�� ������
	float AttackRadius = 50.0f;

	// �浹 Ž���� ���� ���� ���� (�÷��̾� ���� ��ġ + ���� ���� �÷��̾��� CapsuleComponent�� ������ �Ÿ�)
	FVector Start = GetActorLocation() + (GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius());
	// �浹 Ž�� ���� ���� (�������� + ���� ������ ���� �Ÿ�)
	FVector End = Start + (GetActorForwardVector() * AttackRange);
	// �Ķ���� �����ϱ� (Ʈ���̽� �±� : Attack, ������ �浹 ó�� : false, ������ ���� : this) 
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
		// TODO : ������ ����
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

	// Capsule ����� ����� üũ
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 3.0f);
}
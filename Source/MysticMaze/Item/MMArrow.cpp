// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMArrow.h"
#include "Collision/MMCollision.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMMArrow::AMMArrow()
{
	FeatherComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Feather"));
	FeatherComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = FeatherComponent;

	ShaftComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shaft"));
	ShaftComponent->SetupAttachment(FeatherComponent);
	ShaftComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IronComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Iron"));
	IronComponent->SetupAttachment(ShaftComponent);
	IronComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ArrowCollision"));
	ArrowCollision->SetupAttachment(IronComponent);
	ArrowCollision->SetCollisionProfileName(MMWEAPON);
	ArrowCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->InitialSpeed = Speed;
}

void AMMArrow::BeginPlay()
{
	Super::BeginPlay();

	// Projectile Movement Component 비활성화
	MovementComponent->SetActive(false);
}

void AMMArrow::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Event Mapping
	ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMArrow::OnBeginOverlap);
}

void AMMArrow::Fire(FVector TargetLocation)
{
	// 방향 구하기
	FVector LaunchDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();

	// 콜리전 활성화
	ArrowCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// 방향 지정 및 Projectile Movement Component 활성화
	MovementComponent->Velocity = LaunchDirection * MovementComponent->InitialSpeed;
	MovementComponent->Activate();

	// 3초 후 자동 삭제
	SetLifeSpan(3.0f);
}

void AMMArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner == OtherActor) return;

	// 캐릭터의 경우 데미지 전달
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(Owner);
		if (!StatPlayer) return;

		float Damage = StatPlayer->GetStatComponent()->GetAttackDamage();
		bool Critical = FMath::FRand() < (StatPlayer->GetStatComponent()->GetCriticalHitRate() / 100);

		if (Critical)
		{
			Damage *= 2.0f;
		}

		// 데미지 전달
		UGameplayStatics::ApplyDamage(OtherActor, Damage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
	}

	// Projectile Movement Component 비활성화
	MovementComponent->SetActive(false);

	// 맞은 물체에 화살 부착
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	this->AttachToActor(OtherActor, AttachmentRules);
}


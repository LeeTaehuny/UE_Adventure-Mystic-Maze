// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMArrow.h"
#include "Collision/MMCollision.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	
	// 방향 지정 및 Projectile Movement Component 활성화
	MovementComponent->Velocity = LaunchDirection * MovementComponent->InitialSpeed;
	MovementComponent->Activate();

	// 3초 후 자동 삭제
	SetLifeSpan(3.0f);
}

void AMMArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner == OtherActor) return;

	// TODO : 데미지 전달
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappedComp->GetName());

	// Projectile Movement Component 비활성화
	MovementComponent->SetActive(false);

	// 맞은 물체에 화살 부착
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	this->AttachToActor(OtherActor, AttachmentRules);
}


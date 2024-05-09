// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMEnergyBall.h"
#include "Collision/MMCollision.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMMEnergyBall::AMMEnergyBall()
{
	EnergyBallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EnergyBallCollision"));
	EnergyBallCollision->SetSphereRadius(8.0f);
	EnergyBallCollision->SetCollisionProfileName(MMWEAPON);
	RootComponent = EnergyBallCollision;

	EnergyBallParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyBallParticleSystemComponent"));
	EnergyBallParticleSystemComponent->SetupAttachment(EnergyBallCollision);

	HitParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitParticleSystemComponent"));
	HitParticleSystemComponent->SetupAttachment(EnergyBallCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->InitialSpeed = Speed;
	bIsHit = false;
}

// Called when the game starts or when spawned
void AMMEnergyBall::BeginPlay()
{
	Super::BeginPlay();
	
	// Projectile Movement Component 비활성화
	MovementComponent->SetActive(false);

	HitParticleSystemComponent->SetActive(false);
}

void AMMEnergyBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Event Mapping
	EnergyBallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMEnergyBall::OnBeginOverlap);
}

void AMMEnergyBall::Fire(FVector TargetLocation)
{
	FVector LaunchDirection;

	// 방향 구하기
	if ((TargetLocation - Owner->GetActorLocation()).Length() < 300.0f)
	{
		LaunchDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
		LaunchDirection.Z = 0.0f;
	}
	else
	{
		LaunchDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	}
	
	// 방향 지정 및 Projectile Movement Component 활성화
	MovementComponent->Velocity = LaunchDirection * MovementComponent->InitialSpeed;
	MovementComponent->Activate();

	// 3초 후 자동 삭제
	SetLifeSpan(3.0f);
}

void AMMEnergyBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsHit) return;
	if (Owner == OtherActor) return;

	// TODO : 데미지 전달
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappedComp->GetName());

	EnergyBallParticleSystemComponent->SetActive(false);
	HitParticleSystemComponent->SetActive(true);
	bIsHit = true;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMEnergyBall.h"
#include "Collision/MMCollision.h"
#include "Interface/MMStatusInterface.h"
#include "Player/MMStatComponent.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AMMEnergyBall::AMMEnergyBall()
{
	EnergyBallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EnergyBallCollision"));
	EnergyBallCollision->SetSphereRadius(8.0f);
	EnergyBallCollision->SetCollisionProfileName(MMWEAPON);
	RootComponent = EnergyBallCollision;

	EnergyBallParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnergyBallParticleSystemComponent"));
	EnergyBallParticleSystemComponent->SetupAttachment(EnergyBallCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->ProjectileGravityScale = 0.0f;
	bIsHit = false;
}

// Called when the game starts or when spawned
void AMMEnergyBall::BeginPlay()
{
	Super::BeginPlay();
	
	// Projectile Movement Component 비활성화
	MovementComponent->SetActive(false);

	// 3초 후 자동 삭제
	SetLifeSpan(5.0f);
}

void AMMEnergyBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Event Mapping
	EnergyBallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMMEnergyBall::OnBeginOverlap);
}

void AMMEnergyBall::Fire(FVector TargetLocation)
{
	if (!IsValid(Owner)) return;

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
}

void AMMEnergyBall::Fire()
{
	if (!Owner) return;

	// 타겟이 있는 경우 타겟으로 유도 발사
	if (Target && Target->GetRootComponent())
	{
		MovementComponent->bIsHomingProjectile = true;
		MovementComponent->HomingAccelerationMagnitude = 3000.0f;
		MovementComponent->HomingTargetComponent = Target->GetRootComponent();
		MovementComponent->Velocity = FMath::VRandCone(Owner->GetActorUpVector(), 0.5f) * 100.0f;
		MovementComponent->Activate();
	}
	// 타겟이 없는 경우 전방으로 발사
	else
	{
		MovementComponent->Velocity = Owner->GetActorForwardVector() * MovementComponent->InitialSpeed;
		MovementComponent->Activate();
	}
}

void AMMEnergyBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsHit) return;
	if (Owner == OtherActor) return;

	// 캐릭터인 경우 데미지 전달
	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (PlayerCharacter)
	{
		IMMStatusInterface* StatPlayer = Cast<IMMStatusInterface>(Owner);
		if (!StatPlayer) return;

		bool Critical = FMath::FRand() < (StatPlayer->GetStatComponent()->GetCriticalHitRate() / 100);

		// 데미지 전달
		UGameplayStatics::ApplyDamage(OtherActor, Critical ? Damage * 2.0f : Damage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
	}
	
	EnergyBallParticleSystemComponent->SetActive(false);

	// 피격 이펙트 연출
	FTransform Transform;
	Transform.SetLocation(SweepResult.ImpactPoint);
	Transform.SetScale3D(FVector(0.5f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Transform);

	Destroy();
	bIsHit = true;
}


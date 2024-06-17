// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Magic/MMFireBall.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Monster/MMGoblinWizard.h"
#include "Interface/MMStatusInterface.h"

AMMFireBall::AMMFireBall()
{
	HitComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Hit Collision"));
	RootComponent = HitComponent;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("UParticleSystemComponent"));
	Particle->SetupAttachment(RootComponent);
	Particle->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireBall.P_ky_fireBall'"));
	if (ParticleRef.Object)
	{
		Particle->Template = ParticleRef.Object;
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AMMFireBall::BeginPlay()
{
	Super::BeginPlay();

	HitComponent->OnComponentBeginOverlap.AddDynamic(this, &AMMFireBall::FireBallBeginOverlap);
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	Direction = FVector();

	MovementComponent->SetActive(false);

	//MagicController = Cast<AAIController>(GetController());

	
}

void AMMFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Direction != FVector())
	//{
	//	FVector NewLocation = GetActorLocation() * (Direction * 100 * DeltaTime);
	//	SetActorLocation(NewLocation);
	//}
}

void AMMFireBall::FireBallBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Target)
	{
		//UGameplayStatics::ApplyDamage(Target,
		//	100.0f, GetController(),
		//	this,
		//	UDamageType::StaticClass());

		AMMGoblinWizard* Wizard = Cast<AMMGoblinWizard>(Owner);
		if (Wizard)
		{
			UE_LOG(LogTemp, Warning, TEXT("Wizard"));
			IMMStatusInterface* Damage = Cast<IMMStatusInterface>(Wizard);
			if (Damage)
			{
				UE_LOG(LogTemp, Warning, TEXT("Stat"));

				UGameplayStatics::ApplyDamage(Target,
					Damage->GetStatComponent()->GetAttackDamage(), Wizard->GetController(),
					this,
					UDamageType::StaticClass());
			}
		}

		

		FTransform Transform;
		Transform.SetLocation(SweepResult.ImpactPoint);
		Transform.SetScale3D(FVector(0.25f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Transform);

		this->Destroy();
	}
}

void AMMFireBall::SetDirectionLocation(FVector INData)
{
	Direction = Target->GetActorLocation() - INData;
	Direction = Direction.GetSafeNormal();

	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
	MovementComponent->Activate();

	SetLifeSpan(3.0f);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/MMBladeWave.h"
#include "Collision/MMCollision.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMMBladeWave::AMMBladeWave()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBox"));
	CollisionBox->SetBoxExtent(FVector(250.0f, 150.0f, 200.0f));
	CollisionBox->SetCollisionProfileName(MMWEAPON);
	RootComponent = CollisionBox;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(CollisionBox);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UNiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Niagara/NiagaraSkills/BladeSlash/NS_BladeSlash.NS_BladeSlash'"));
	if (UNiagaraRef.Object)
	{
		NiagaraComponent->SetAsset(UNiagaraRef.Object);
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->InitialSpeed = 1500.0f;
}

void AMMBladeWave::BeginPlay()
{
	Super::BeginPlay();
	
	// 3초 후 자동 삭제
	SetLifeSpan(3.0f);
}

void AMMBladeWave::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMMBladeWave::OnBeginOverlap);
}

void AMMBladeWave::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner == OtherActor) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
	if (!PlayerCharacter) return;

	// 치명타 체크하기
	bool Critical = FMath::FRand() < (CriticalRate / 100);
	// 데미지 전달
	UGameplayStatics::ApplyDamage(OtherActor, Critical ? Damage * 2.0f : Damage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/MMStome.h"
#include "Collision/MMCollision.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMMStome::AMMStome()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	RootComponent = ParticleComponent;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> StormParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_aquaStorm.P_ky_aquaStorm'"));
	if (StormParticleRef.Object)
	{
		ParticleComponent->SetTemplate(StormParticleRef.Object);
	}

	IntervalTime = 0.5f;
	SumTime = 0.0f;
	DestroyTime = 3.0f;
}

// Called when the game starts or when spawned
void AMMStome::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(DestroyTime);
}

// Called every frame
void AMMStome::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SumTime += DeltaTime;
	if (SumTime >= IntervalTime)
	{
		SumTime = 0.0f;
		AttackCheck();
	}
}

void AMMStome::AttackCheck()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params;

	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		CHANNEL_MMACTION,
		FCollisionShape::MakeSphere(700.0f),
		Params
	);

	if (bHasHit)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Owner);
		if (!PlayerCharacter) return;

		// 플레이어를 제외한 나머지에게 데미지 전달
		for (FOverlapResult Result : OverlapResults)
		{
			// 플레이어의 경우 건너 뛰기
			if (Result.GetActor() == Owner) continue;

			// 데미지 전달
			if (Cast<ACharacter>(Result.GetActor()))
			{
				// 치명타 체크하기
				bool Critical = FMath::FRand() < (CriticalRate / 100);

				UGameplayStatics::ApplyDamage(Result.GetActor(), Critical ? Damage * 2.0f : Damage, PlayerCharacter->GetController(), Owner, UDamageType::StaticClass());
			}
		}
	}
}


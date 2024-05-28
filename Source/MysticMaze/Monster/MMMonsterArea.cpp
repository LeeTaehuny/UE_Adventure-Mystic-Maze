// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterArea.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMMMonsterArea::AMMMonsterArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMMMonsterArea::AddMonsterData(AMMMonsterBase* INData)
{
	MonsterDAta.Add(INData);
}

void AMMMonsterArea::IfMonsterNull(float DeltaTime)
{
	// 3초마다 탐색
	player_Serch_timer += DeltaTime;
	if (player_Serch_timer >= 0.5f)
	{
		player_Serch_timer = 0;
		MonsterDAta.RemoveAll([](AMMMonsterBase* Monster)
			{
				return Monster == nullptr || Monster->IsPendingKill();
			});

		// 몬스터 배열에 아무도 없으면 해당 클래스 삭제
		if (MonsterDAta.Num() <= 0)
		{
			this->Destroy();
			return;
		}

		float Distance = FVector::Distance(Player->GetActorLocation(), CheckLocation);
		if (Distance >= 4000)
		{
			for (int i = 0; i < MonsterDAta.Num(); i++)
			{
				MonsterDAta[i]->SetATKMode(false);

				// 만약 몬스터가 버그스웜일 경우
				
			}
		}
	}
}

// Called when the game starts or when spawned
void AMMMonsterArea::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AMMMonsterArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IfMonsterNull(DeltaTime);
}


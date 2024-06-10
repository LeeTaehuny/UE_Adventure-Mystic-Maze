// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMMonsterArea.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMMMonsterArea::AMMMonsterArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

void AMMMonsterArea::AddMonsterData(AMMMonsterBase* INData)
{
	MonsterDAta.Add(INData);
}

bool AMMMonsterArea::IfMonsterNull(float DeltaTime)
{
	// 3초마다 탐색
	player_Serch_timer += DeltaTime;
	if (player_Serch_timer >= 0.5f)
	{
		// 몬스터 배열에 아무도 없으면 해당 클래스 삭제
		if (MonsterDAta.Num() == 0)
		{
			//this->Destroy();
			return true;
		}

		player_Serch_timer = 0;
		//MonsterDAta.RemoveAll([](AMMMonsterBase* Monster)
		//	{
		//		return Monster == nullptr || Monster->IsPendingKill();
		//	});

		TArray<AMMMonsterBase*> TempMonsterData = MonsterDAta;
		TempMonsterData.RemoveAll([](AMMMonsterBase* Monster)
			{
				return Monster == nullptr || Monster->IsPendingKill();
			});
		MonsterDAta = TempMonsterData;

		float Distance = FVector::Distance(Player->GetActorLocation(), CheckLocation);
		if (Distance >= 5600)
		{
			for (int i = 0; i < MonsterDAta.Num(); i++)
			{
				MonsterDAta[i]->SetATKMode(false);
			}
		}

		// 만약 한 마리라도 전투 모두에 돌입을 한 몬스터가 있다고 한다면
		// 모든 몬스터들을 전투 모두로 전환
		for (int i = 0; i < MonsterDAta.Num(); i++)
		{
			if (MonsterDAta[i]->GetATKMode())
			{
				for (int x = 0; x < MonsterDAta.Num(); x++)
				{
					MonsterDAta[x]->SetATKMode(true);
				}
				break;
			}
		}
	}

	return false;
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

	//IfMonsterNull(DeltaTime);
}


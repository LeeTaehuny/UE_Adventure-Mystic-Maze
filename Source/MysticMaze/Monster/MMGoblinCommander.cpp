// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MMGoblinCommander.h"

#include "Monster/MMGoblinWarrior.h"
#include "Monster/MMGoblinWizard.h"

#include "Collision/MMCollision.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

// Sets default values
AMMGoblinCommander::AMMGoblinCommander()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMMGoblinCommander::BeginPlay()
{
	Super::BeginPlay();
	
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AMMGoblinCommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickTime += DeltaTime;
	if (TickTime >= 1.0f)
	{
		TickTime = 0.0f;

		DataReset();
		GoblinBattleAlgorithm(DeltaTime);
	}
}

void AMMGoblinCommander::GoblinBattleAlgorithm(float DeltaTime)
{
	// 파티 안에 마법사가 있는지 판별하기 위한 if문
	if (WizardDatas.Num() != 0)
	{
		// 전체 전사중 일부만을 보호 역할로 전환
		// 만약 전사가 2마리 이하일 경우 마법사들을 보호하지 않음
		if (WarriorDatas.Num() <= 2)
		{

		}
		else if (WarriorDatas.Num() < 4)
		{
			// 만약 파이에 포함된 전체 전사가 4마리 미만일 경우
			// 보호역할을 수행하는 고블린은 1마리
			for (int i = 0; i < 1; i++)
			{
				AAIController* WarriorController = Cast<AAIController>(WarriorDatas[i]->GetController());
				if (WarriorController)
				{
					WarriorController->GetBlackboardComponent()->SetValueAsBool("Protector", true);
					WarriorController->GetBlackboardComponent()->SetValueAsBool("Attacker", false);
				}
			}
		}
		else // 4마리 이상인 경우 보호역할을 수행하는 고블린은 2마리
		{
			for (int i = 0; i < 2; i++)
			{
				AAIController* WarriorController = Cast<AAIController>(WarriorDatas[i]->GetController());
				if (WarriorController)
				{
					WarriorController->GetBlackboardComponent()->SetValueAsBool("Protector", true);
					WarriorController->GetBlackboardComponent()->SetValueAsBool("Attacker", false);
				}
			}
		}

		for (int i = 0; i < WizardDatas.Num(); i++)
		{
			AAIController* WizardController = Cast<AAIController>(WizardDatas[i]->GetController());
			if (WizardController)
			{
				WizardController->GetBlackboardComponent()->SetValueAsBool("Dealer", true);
				WizardController->GetBlackboardComponent()->SetValueAsBool("Support", false);
			}
		}
	}

	float minDistance = FLT_MAX;
	int Number = 0;
	for (int i = 0; i < WizardDatas.Num(); i++)
	{
		float curDistance = FVector::Distance(WizardDatas[i]->GetActorLocation(), TargetPlayer->GetActorLocation());

		if (curDistance <= minDistance)
		{
			Number = i;
			minDistance = curDistance;

			UE_LOG(LogTemp, Display, TEXT(""));
		}
	}

	int Count = 0;
	for (int i = 0; i < WarriorDatas.Num(); i++)
	{
		AAIController* WarriorController = Cast<AAIController>(WarriorDatas[i]->GetController());
		if (WarriorController)
		{
			if (WarriorController->GetBlackboardComponent()->GetValueAsBool("Protector"))
			{
				Count++;

				FVector Direction = TargetPlayer->GetActorLocation() - WizardDatas[Number]->GetActorLocation();
				Direction = Direction.GetSafeNormal();
				FVector Protect = WizardDatas[Number]->GetActorLocation() + (Direction * 300);

				WarriorController->GetBlackboardComponent()->SetValueAsVector("ProtectLocation", Protect);

				UE_LOG(LogTemp, Display, TEXT("X : %f, Y : %f, Z : %f"), Protect.X, Protect.Y, Protect.Z);
			}
			else
			{
				WarriorController->GetBlackboardComponent()->SetValueAsBool("Attacker", true);
			}
		}
	}
}

void AMMGoblinCommander::DataReset()
{
	WarriorDatas.RemoveAll([](AMMMonsterBase* Monster)
		{
			return Monster == nullptr || Monster->IsPendingKill();
		});
	WizardDatas.RemoveAll([](AMMMonsterBase* Monster)
		{
			return Monster == nullptr || Monster->IsPendingKill();
		});
}

void AMMGoblinCommander::SerchGoblin(FVector INData)
{
	// 받아온 벡터의 값을 중심으로 범위 안에 있는 다른 액터들의 정보를 받아와서
	// 데이터를 변형하여 데이터 변형이 되면 데이터 저장

	TArray<FOverlapResult> Overlaps;
	FQuat Rotation = FQuat::Identity; // 회전 없음
	ECollisionChannel TraceChannel = ECC_WorldStatic; // 충돌 채널
	//FCollisionShape CollisionShape = FCollisionShape::MakeSphere(8000.0f); 
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(4000, 4000, 1000));
	FCollisionQueryParams Params;

	if (GetWorld()->OverlapMultiByChannel(
		Overlaps,		// 충돌한 액터들의 정보
		INData,	// 탐색할 위치
		Rotation,		// 회전 정보 (회전 없음)
		CHANNEL_MMACTION,	// 충돌 채널
		CollisionShape, // 충돌체
		Params))
	{
		for (FOverlapResult& Data : Overlaps)
		{
			if (AMMGoblinWarrior* WarriorType = Cast<AMMGoblinWarrior>(Data.GetActor()))
			{
				WarriorDatas.Add(WarriorType);
				continue;
			}
			else if (AMMGoblinWizard* WizardType = Cast<AMMGoblinWizard>(Data.GetActor()))
			{
				WizardDatas.Add(WizardType);
				continue;
			}
		}
	}
}
void AMMGoblinCommander::SerchGoblin(AActor* INData)
{
	// 액터의 정보를 받아서 각 타입에 맞는 클래스로 변형하고 데이터를 저장하기 위한 함수

	if (AMMGoblinWarrior* WarriorType = Cast<AMMGoblinWarrior>(INData))
	{
		WarriorDatas.Add(WarriorType);
	}
	else if (AMMGoblinWizard* WizardType = Cast<AMMGoblinWizard>(INData))
	{
		WizardDatas.Add(WizardType);
	}
}

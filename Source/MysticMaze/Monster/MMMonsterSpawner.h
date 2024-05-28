// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster/MMMonsterSpawnType.h"

#include "MMMonsterSpawner.generated.h"

UCLASS()
class MYSTICMAZE_API AMMMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMMonsterSpawner();

	void MonsterSpawn(SpawnType INType, int INLevel, FVector INCenterLocation, FVector INOriginCenterLocation = FVector());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	SpawnType MType = SpawnType::BugswarmOnly;

	// ������ ������ �������Ʈ �����͸� �����ϱ� ���� ������
	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMBugSwarm> BugSwarmData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMMechanite> MechaniteData;

	UPROPERTY(EditAnywhere, Category = "MonsterData")
	TSubclassOf<class AMMGrux> GruxData;

	TArray<TObjectPtr<class AMMMonsterArea>> Areas;

	// TArray<TObjectPtr<class AMMMonsterBase>> Monster;

	// ���� �߽� ����Ʈ�� ����ϱ� ���� ���� ����
	// �̰��� �߽����� ���͵��� ��濡 ������
	// �迭�� ũ�⿡ ���� ��ȯ�Ǵ� ��ġ�� �� �ٸ�
	// FVector SpawnPoint;
};

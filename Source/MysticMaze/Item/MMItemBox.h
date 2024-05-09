// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMItemBox.generated.h"

UCLASS()
class MYSTICMAZE_API AMMItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AMMItemBox();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// 아이템 박스에 아이템 데이터를 추가하기 위한 함수
	void AddItemList(TMap<FString, int32> InItemList);
	// 아이템 박스에 골드를 추가하기 위한 함수
	void AddMoney(int32 InMoney);

protected:
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TMap<TObjectPtr<class UMMItemData>, int32> Items;

	UPROPERTY(VisibleAnywhere, Category = Item)
	int32 Gold;
};

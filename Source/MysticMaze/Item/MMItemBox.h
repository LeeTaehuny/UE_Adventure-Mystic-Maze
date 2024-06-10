// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MMInteractionInterface.h"
#include "MMItemBox.generated.h"

UCLASS()
class MYSTICMAZE_API AMMItemBox : public AActor, public IMMInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AMMItemBox();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	FORCEINLINE int32 GetItemQuantity() { return ItemQuantity; }
	FORCEINLINE int32 GetGold() { return Gold; }
	FORCEINLINE FName GetItemName() { return ItemName; }

	// 아이템 박스에 아이템 데이터를 추가하기 위한 함수
	void AddItemQuantity(int32 InQuantity);
	// 아이템 박스에 골드를 추가하기 위한 함수
	void AddMoney(int32 InMoney);

protected:
	// 상호작용 함수
	virtual void Interaction(ACharacter* PlayerCharacter) override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

private:
	UPROPERTY(EditAnywhere, Category = Item)
	int32 ItemQuantity;

	UPROPERTY(VisibleAnywhere, Category = Item)
	int32 Gold;

	UPROPERTY(VisibleAnywhere, Category = Item)
	FName ItemName;
	FString HelpText;
};

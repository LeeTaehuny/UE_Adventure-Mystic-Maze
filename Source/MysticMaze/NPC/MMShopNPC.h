// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/MMNPCBase.h"
#include "Interface/MMInteractionInterface.h"
#include "MMShopNPC.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMShopNPC : public AMMNPCBase, public IMMInteractionInterface
{
	GENERATED_BODY()
	
public:
	AMMShopNPC();

public:
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetItems() { return ShopItems; }

	// 상점 닫기 함수
	void CloseShop();

	// 아이템 구매 함수
	void Purchase(int32 ItemIndex);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// 상호작용 함수
	virtual void Interaction(ACharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ShopWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMMShopWidget> ShopWidget;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMItemData>> ShopItemData;

	UPROPERTY()
	TArray<TObjectPtr<class UMMInventoryItem>> ShopItems;

private:
	FString HelpText;

	UPROPERTY()
	TObjectPtr<ACharacter> TempPlayer;
	
};

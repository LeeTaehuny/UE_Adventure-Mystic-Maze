// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/MMNPCBase.h"
#include "Interface/MMInteractionInterface.h"
#include "GameData/MMEnums.h"
#include "MMBlacksmithNPC.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMBlacksmithNPC : public AMMNPCBase, public IMMInteractionInterface
{
	GENERATED_BODY()
	
public:
	AMMBlacksmithNPC();

public:
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetWarriorItems() { return ShopWarriorItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetArcherItems() { return ShopArcherItems; }
	FORCEINLINE TArray<TObjectPtr<class UMMInventoryItem>> GetMageItems() { return ShopMageItems; }

	// 상점 닫기 함수
	void CloseShop();

	// 아이템 구매 함수
	void Purchase(ESlotType Type, int32 ItemIndex);

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
	TObjectPtr<class UMMBlacksmithWidget> ShopWidget;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMItemData>> WarriorItemData;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMItemData>> ArcherItemData;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMItemData>> MageItemData;

	UPROPERTY()
	TArray<TObjectPtr<class UMMInventoryItem>> ShopWarriorItems;

	UPROPERTY()
	TArray<TObjectPtr<class UMMInventoryItem>> ShopArcherItems;

	UPROPERTY()
	TArray<TObjectPtr<class UMMInventoryItem>> ShopMageItems;

private:
	FString HelpText;

	UPROPERTY()
	TObjectPtr<ACharacter> TempPlayer;
};

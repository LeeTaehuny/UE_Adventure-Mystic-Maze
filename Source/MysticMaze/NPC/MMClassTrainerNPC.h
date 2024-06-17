// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/MMNPCBase.h"
#include "Interface/MMInteractionInterface.h"
#include "GameData/MMEnums.h"
#include "MMClassTrainerNPC.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API AMMClassTrainerNPC : public AMMNPCBase, public IMMInteractionInterface
{
	GENERATED_BODY()
	
public:
	AMMClassTrainerNPC();

public:
	FORCEINLINE TArray<TObjectPtr<class UMMSkillData>> GetWarriorSkillData() { return WarriorSkillData; }
	FORCEINLINE TArray<TObjectPtr<class UMMSkillData>> GetArcherSkillData() { return ArcherSkillData; }
	FORCEINLINE TArray<TObjectPtr<class UMMSkillData>> GetMageSkillData() { return MageSkillData; }
	FORCEINLINE TMap<FString, int32> GetWarriorSkillMap() { return WarriorSkillMap; }
	FORCEINLINE TMap<FString, int32> GetArcherSkillMap() { return ArcherSkillMap; }
	FORCEINLINE TMap<FString, int32> GetMageSkillMap() { return MageSkillMap; }
	FORCEINLINE int32 GetPlayerLevel() { return PlayerLevel; }

	// 상점 닫기 함수
	void CloseShop();

	// 스킬 구매 함수
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

	void UpdateSkillMap();
	void UpdateWarriorSkillMap();
	void UpdateArcherSkillMap();
	void UpdateMageSkillMap();

private:
	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ClassTrainerWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMMClassTrainerWidget> ClassTrainerWidget;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMSkillData>> WarriorSkillData;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMSkillData>> ArcherSkillData;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UMMSkillData>> MageSkillData;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> WarriorSkillMap;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> ArcherSkillMap;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> MageSkillMap;


private:
	FString HelpText;

	UPROPERTY()
	TObjectPtr<ACharacter> TempPlayer;

	int32 PlayerLevel;
};

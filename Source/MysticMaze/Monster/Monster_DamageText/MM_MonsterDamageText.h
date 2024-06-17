// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "MM_MonsterDamageText.generated.h"

UCLASS()
class MYSTICMAZE_API AMM_MonsterDamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMM_MonsterDamageText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DamageText(float INData, FVector INActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* TextRenderComponent;

	//TObjectPtr<AActor> TargetPlayerData;
	FVector TargetPlayerData;

};

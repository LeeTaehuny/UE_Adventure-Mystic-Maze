// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MMInteractionInterface.h"
#include "MMPortal.generated.h"

UCLASS()
class MYSTICMAZE_API AMMPortal : public AActor, public IMMInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AMMPortal();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Particle;

	// 상호작용 함수
	virtual void Interaction(ACharacter* PlayerCharacter) override;

private:
	void OpenLevel();

	FString HelpText;
	
	UPROPERTY(EditAnywhere, Category = "Portal", meta = (AllowPrivateAccess = "true"))
	FName OpenLevelName;

	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	FTimerHandle LoadingTimerHandle;
};

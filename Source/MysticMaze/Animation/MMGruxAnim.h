// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/MMBaseAnimInstance.h"
#include "MMGruxAnim.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICMAZE_API UMMGruxAnim : public UMMBaseAnimInstance
{
	GENERATED_BODY()

public:
	UMMGruxAnim();

protected:
	// Initialize
	virtual void NativeInitializeAnimation() override;
	// Update
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bDie : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bSpawn : 1;

	// 여러개의 죽음 모션을 실행시키기 위한 변수
	// 랜덤으로 참과 거짓을 반환하는데 어차피 2개의 값만 있으면 되기에 채택
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimBP")
	uint8 bRandomDieMotion : 1;
	
};

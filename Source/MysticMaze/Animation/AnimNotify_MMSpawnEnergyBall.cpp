// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMSpawnEnergyBall.h"
#include "Interface/MMAnimationWeaponInterface.h"

void UAnimNotify_MMSpawnEnergyBall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// TODO : 에너지볼 스폰하기
}

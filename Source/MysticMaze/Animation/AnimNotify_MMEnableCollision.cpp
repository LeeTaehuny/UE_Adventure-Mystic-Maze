﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMEnableCollision.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMWeapon.h"

void UAnimNotify_MMEnableCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 콜리전 On
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn)
		{
			WeaponPawn->GetWeapon()->EnableCollision();
		}
	}
}

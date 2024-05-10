// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMArrowSpawn.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMBowWeapon.h"

void UAnimNotify_MMArrowSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 화살을 스폰합니다.
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn)
		{
			AMMBowWeapon* BowWeapon = Cast<AMMBowWeapon>(WeaponPawn->GetWeapon());
			if (BowWeapon)
			{
				BowWeapon->SpawnArrow();
			}
		}
	}
}

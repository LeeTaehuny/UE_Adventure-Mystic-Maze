// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMEquipWeapon.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMWeapon.h"

void UAnimNotify_MMEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 무기 장착
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn && WeaponPawn->GetWeapon())
		{
			WeaponPawn->GetWeapon()->DrawWeapon(MeshComp);
		}
	}
}

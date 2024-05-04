// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMDisableCollision.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMSwordWeapon.h"

void UAnimNotify_MMDisableCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 콜리전 Off
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn)
		{
			AMMSwordWeapon* SwordWeapon = Cast<AMMSwordWeapon>(WeaponPawn->GetWeapon());
			if (SwordWeapon)
			{
				SwordWeapon->DisableCollision();
			}
		}
	}
}

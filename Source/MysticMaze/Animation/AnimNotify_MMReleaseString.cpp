// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMReleaseString.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMBowWeapon.h"

void UAnimNotify_MMReleaseString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 활을 놓았다는 것을 전달합니다.
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn)
		{
			AMMBowWeapon* BowWeapon = Cast<AMMBowWeapon>(WeaponPawn->GetWeapon());
			if (BowWeapon)
			{
				BowWeapon->SetIsHold(false);
			}
		}
	}
}

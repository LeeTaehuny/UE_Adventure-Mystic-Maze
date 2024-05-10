// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMPullString.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMBowWeapon.h"

void UAnimNotify_MMPullString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 활을 당겼다는 것을 전달합니다.
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn)
		{
			AMMBowWeapon* BowWeapon = Cast<AMMBowWeapon>(WeaponPawn->GetWeapon());
			if (BowWeapon)
			{
				BowWeapon->SetIsHold(true);
			}
		}
	}
}

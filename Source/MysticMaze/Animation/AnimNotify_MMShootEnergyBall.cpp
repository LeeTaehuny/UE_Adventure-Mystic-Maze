// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMShootEnergyBall.h"
#include "Interface/MMAnimationWeaponInterface.h"
#include "Item/MMStaffWeapon.h"

void UAnimNotify_MMShootEnergyBall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 에너지볼을 발사합니다.
		IMMAnimationWeaponInterface* WeaponPawn = Cast<IMMAnimationWeaponInterface>(MeshComp->GetOwner());
		if (WeaponPawn)
		{
			AMMStaffWeapon* StaffWeapon = Cast<AMMStaffWeapon>(WeaponPawn->GetWeapon());
			if (StaffWeapon)
			{
				StaffWeapon->ShootEnergyBall();
			}
		}
	}
}

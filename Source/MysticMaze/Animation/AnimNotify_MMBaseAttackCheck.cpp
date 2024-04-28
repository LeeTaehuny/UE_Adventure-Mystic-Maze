// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMBaseAttackCheck.h"
#include "Interface/MMAnimationAttackInterface.h"

void UAnimNotify_MMBaseAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 공격 성공 여부 체크
		IMMAnimationAttackInterface* AttackPawn = Cast<IMMAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->BaseAttackCheck();
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMMonsterATK.h"
#include "Interface/MMMonsterATKChekInterface.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_MMMonsterATK::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IMMMonsterATKChekInterface* Monster = Cast<IMMMonsterATKChekInterface>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->ATKChecking();
	}
}

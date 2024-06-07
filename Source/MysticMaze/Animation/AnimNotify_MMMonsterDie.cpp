// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMMonsterDie.h"

#include "Interface/MMMonsterDieInterface.h"

void UAnimNotify_MMMonsterDie::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IMMMonsterDieInterface* Monster = Cast<IMMMonsterDieInterface>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->Monsterdie();
	}
}
﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMSkillAttackEnd.h"
#include "Interface/MMSkillInterface.h"

#include "Player/MMSkillComponent.h"
#include "Skill/MMSkillBase.h"

void UAnimNotify_MMSkillAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMMSkillInterface* SkillPawn = Cast<IMMSkillInterface>(MeshComp->GetOwner());
		if (SkillPawn)
		{
			SkillPawn->GetSkillComponent()->GetUsingSkill()->SkillAttackEnd();
		}
	}
}
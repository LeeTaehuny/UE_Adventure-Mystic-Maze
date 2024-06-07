// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_WizardNormalATK.h"

#include "Interface/MMWizardNormalATKInterface.h"

void UAnimNotify_WizardNormalATK::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IMMWizardNormalATKInterface* Wizard = Cast<IMMWizardNormalATKInterface>(MeshComp->GetOwner());
	if (Wizard)
	{
		Wizard->StartATK();
	}
}

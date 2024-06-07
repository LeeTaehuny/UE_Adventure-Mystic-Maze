// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMGruxModeComplete.h"

#include "Interface/MMGruxModeChangeCPInterface.h"

void UAnimNotify_MMGruxModeComplete::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 애니메이션의 모드 변경이 제대로 끝났을 경우 신호를 주기 위한 노티파이

	IMMGruxModeChangeCPInterface* Grux = Cast<IMMGruxModeChangeCPInterface>(MeshComp->GetOwner());
	if (Grux)
	{
		UE_LOG(LogTemp, Display, TEXT("Complete"));
		Grux->SetMode(true);
	}
}

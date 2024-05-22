// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MMMonsterSpawn.h"
#include "Interface/MMMonsterSpawnInterface.h"

void UAnimNotify_MMMonsterSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 스폰 애니메이션이 끝났을 경우 호출
	if (MeshComp)
	{
		IMMMonsterSpawnInterface* monster = Cast<IMMMonsterSpawnInterface>(MeshComp->GetOwner());
		if (monster)
		{
			// 조건이 맞을 경우 스폰 변수를 false로 변경
			// 애니메이션이 다음으로 넘어갈 수 있도록 해줍니다.
			monster->SetSpawnData(false);
		}
	}
}

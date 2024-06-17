// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMWidgetComponent.h"

#include "UI/MMMonsterHPBar.h"

void UMMWidgetComponent::InitWidget()
{
    Super::InitWidget();

    // 위젯에 대한 인스턴스가 생성이 되었으므로 관련된 액터 정보를 얻어 OwningActor에 저장하기
    // * WidgetComponent 내부의 Widget을 가져옵니다.
	UMMMonsterHPBar* HP = Cast<UMMMonsterHPBar>(GetWidget());
	if (HP)
	{
		// * 소유한 액터의 정보를 저장합니다.
		HP->SetOwningActor(GetOwner());
	}
}

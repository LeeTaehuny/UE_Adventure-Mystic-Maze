// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MMMonsterHPBar.h"

#include "Components/ProgressBar.h"
#include "Interface/MMWidgetInterface.h"

void UMMMonsterHPBar::Init()
{
	//UpdateHPBar();
}

void UMMMonsterHPBar::UpdateHPBar(float CurHP, float MaxHP)
{
	float Per = CurHP / MaxHP;

	MonsterHPBar->SetPercent(Per);
}

void UMMMonsterHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	IMMWidgetInterface* HPBar = Cast<IMMWidgetInterface>(OwningActor);
	if (HPBar)
	{
		HPBar->SetupCharacterWidget(this);
	}

	if (MonsterHPBar)
	{
		MonsterHPBar->SetPercent(1.0f);
	}
}

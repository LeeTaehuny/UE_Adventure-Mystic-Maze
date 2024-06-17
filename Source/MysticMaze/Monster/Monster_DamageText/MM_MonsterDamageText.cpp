// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster_DamageText/MM_MonsterDamageText.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMM_MonsterDamageText::AMM_MonsterDamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Damage Component"));
    TextRenderComponent->SetupAttachment(RootComponent);

    // Set some properties for the text render component
    TextRenderComponent->SetText(FText::FromString("Hello, Unreal!"));
    TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
    TextRenderComponent->SetVerticalAlignment(EVRTA_TextCenter);
    TextRenderComponent->SetTextRenderColor(FColor::Red);
    TextRenderComponent->SetWorldSize(50.0f);
}

// Called when the game starts or when spawned
void AMM_MonsterDamageText::BeginPlay()
{
	Super::BeginPlay();
	
    SetLifeSpan(1.0f);
}

// Called every frame
void AMM_MonsterDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector CurLocation = GetActorLocation();
    CurLocation.Z += 100 * DeltaTime;
    SetActorLocation(CurLocation);

    //if (TargetPlayerData)
    {
        FVector PlayerLocation = TargetPlayerData;
        FVector MyLocation = this->GetActorLocation();

        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, PlayerLocation);
        
        SetActorRotation(NewRotation);
        //SetActorRotation(TargetPlayerData);
    }
}

void AMM_MonsterDamageText::DamageText(float INData, FVector INActor)
{
    TextRenderComponent->SetText(FText::AsNumber(INData));
    TargetPlayerData = INActor;
}


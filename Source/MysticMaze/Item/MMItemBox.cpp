// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMItemBox.h"
#include "Collision/MMCollision.h"
#include "Item/MMItemData.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMMItemBox::AMMItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(MMTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MysticMaze/Items/ItemBox/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	ItemName = TEXT("DA_ManaStone");
}

// Called when the game starts or when spawned
void AMMItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMMItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMMItemBox::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AMMItemBox::OnEndOverlap);
}

void AMMItemBox::AddItemQuantity(int32 InQuantity)
{
	if (InQuantity < 1) return;

	ItemQuantity = InQuantity;
}

void AMMItemBox::AddMoney(int32 InMoney)
{
	if (InMoney < 0) return;

	Gold = InMoney;
}

void AMMItemBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap"));

	// TODO : 플레이어 상호작용 가능하도록 체크
}

void AMMItemBox::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));

	// TODO : 플레이어 상호작용 불가능하도록 체크해제
}

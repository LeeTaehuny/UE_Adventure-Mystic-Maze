// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMItemBox.h"
#include "Collision/MMCollision.h"
#include "Item/MMItemData.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/AssetManager.h"

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
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
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

void AMMItemBox::AddItemList(TMap<FString, int32> InItemList)
{
	// 애셋 매니저 생성
	UAssetManager& Manager = UAssetManager::Get();

	// 애셋 아이디 리스트 받아오기
	TArray<FPrimaryAssetId> Assets;
	// * 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 반환받음
	Manager.GetPrimaryAssetIdList(TEXT("MMItemData"), Assets);

	// 받아온 목록이 존재한다면?
	if (0 < Assets.Num())
	{
		// 애셋 맵 생성
		TMap<FString, FPrimaryAssetId> AssetMap;
		
		// 애셋 리스트를 맵에 추가
		for (FPrimaryAssetId AssetId : Assets)
		{
			FString AssetName = AssetId.PrimaryAssetName.ToString();
			AssetMap.Add(AssetName, AssetId);
		}

		// InItemNames에 있는 애셋들을 찾아서 처리
		for (const auto& InItem : InItemList)
		{
			if (AssetMap.Find(InItem.Key))
			{
				FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(AssetMap[InItem.Key]));
				if (AssetPtr.IsPending())
				{
					AssetPtr.LoadSynchronous();
				}

				if (AssetPtr.Get())
				{
					// 아이템 목록에 추가
					Items.Add(TPair<UMMItemData*, int32>(Cast<UMMItemData>(AssetPtr.Get()), InItem.Value));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Item Add Error"));
				}
			}
		}
	}
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

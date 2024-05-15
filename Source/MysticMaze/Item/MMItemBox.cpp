// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MMItemBox.h"
#include "Collision/MMCollision.h"
#include "Item/MMItemData.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMPlayerController.h"

#include "GameFramework/Character.h"
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
	Trigger->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/MysticMaze/Items/ItemBox/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	ItemName = TEXT("DA_ManaStone");
	HelpText = TEXT("Press 'F' to pick up the item.");
}

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
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->InteractionWidgetHelpText(HelpText);
			PlayerController->ToggleInteractionVisibility(true);
		}
	}
}

void AMMItemBox::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 비활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->ToggleInteractionVisibility(false);
		}
	}
}

void AMMItemBox::Interaction(ACharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	IMMInventoryInterface* InvPlayer = Cast<IMMInventoryInterface>(PlayerCharacter);
	if (InvPlayer)
	{
		// 줍기 애니메이션 실행
		PlayerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(InvPlayer->GetPickUpMontage(), 1.5f);
	
		// 남은 아이템의 수량을 저장하기 위한 변수
		int32 TempItemQuantity = 0;

		// 인벤토리에 추가 가능한지 여부 확인하며 인벤토리에 추가하기
		if (InvPlayer->GetInventoryComponent()->AddItem(ItemName, ItemQuantity, TempItemQuantity))
		{
			// 성공적으로 추가한 경우 골드를 인벤토리에 추가하기
			InvPlayer->GetInventoryComponent()->AddGold(Gold);

			// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 비활성화 시켜줍니다.
			AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
			if (PlayerController)
			{
				PlayerController->ToggleInteractionVisibility(false);
			}

			// 상자 소멸시키기
			Destroy();
		}
		// 인벤토리 공간이 모자라서 추가하지 못한 경우
		else
		{
			// 골드만 인벤토리에 추가하고 아이템 수량 재설정하기
			InvPlayer->GetInventoryComponent()->AddGold(Gold);
			Gold = 0;
			ItemQuantity = TempItemQuantity;
		}
	}
}

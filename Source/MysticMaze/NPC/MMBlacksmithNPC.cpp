// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/MMBlacksmithNPC.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"
#include "Player/MMPlayerController.h"
#include "UI/MMHUDWidget.h"
#include "UI/NPC/MMBlacksmithWidget.h"
#include "Item/MMItemData.h"
#include "Item/MMWeaponItemData.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"

AMMBlacksmithNPC::AMMBlacksmithNPC()
{
	HelpText = TEXT("Press F to use the Blacksmith.");
}

void AMMBlacksmithNPC::BeginPlay()
{
	Super::BeginPlay();
}

void AMMBlacksmithNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMMBlacksmithNPC::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AMMBlacksmithNPC::OnEndOverlap);

	// 아이템 생성
	for (const auto& ItemData : WarriorItemData)
	{
		if (ItemData)
		{
			UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
			NewItem->ItemData = ItemData;
			NewItem->ItemQuantity = 1;

			ShopWarriorItems.Add(NewItem);
		}
	}

	for (const auto& ItemData : ArcherItemData)
	{
		if (ItemData)
		{
			UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
			NewItem->ItemData = ItemData;
			NewItem->ItemQuantity = 1;

			ShopArcherItems.Add(NewItem);
		}
	}

	for (const auto& ItemData : MageItemData)
	{
		if (ItemData)
		{
			UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
			NewItem->ItemData = ItemData;
			NewItem->ItemQuantity = 1;

			ShopMageItems.Add(NewItem);
		}
	}
}

void AMMBlacksmithNPC::CloseShop()
{
	if (TempPlayer && ShopWidget)
	{
		// 게임모드로 전환
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(TempPlayer->GetController());
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = false;
			ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		// 판매 기능 비활성화
		IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(TempPlayer);
		if (InventoryPawn)
		{
			InventoryPawn->GetInventoryComponent()->SetSellable(false);
		}

		TempPlayer = nullptr;
	}
}

void AMMBlacksmithNPC::Purchase(ESlotType Type, int32 ItemIndex)
{
	if (TempPlayer)
	{
		IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(TempPlayer);
		if (InventoryPawn)
		{
			UMMInventoryItem* TempItem = nullptr;

			// 타입에 따라 아이템이 정확한지 확인하고 아이템 정보 저장하기
			switch (Type)
			{
			case ESlotType::ST_ShopWarriorWeapon:
				if (ShopWarriorItems.IsValidIndex(ItemIndex) && IsValid(ShopWarriorItems[ItemIndex]))
				{
					TempItem = ShopWarriorItems[ItemIndex];
				}
				break;

			case ESlotType::ST_ShopArcherWeapon:
				if (ShopArcherItems.IsValidIndex(ItemIndex) && IsValid(ShopArcherItems[ItemIndex]))
				{
					TempItem = ShopArcherItems[ItemIndex];
				}
				break;

			case ESlotType::ST_ShopMageWeapon:
				if (ShopMageItems.IsValidIndex(ItemIndex) && IsValid(ShopMageItems[ItemIndex]))
				{
					TempItem = ShopMageItems[ItemIndex];
				}
				break;
			}

			// 아이템이 존재하는 경우 구매 가능한지 체크하기
			if (IsValid(TempItem))
			{
				UMMWeaponItemData* WeaponData = Cast<UMMWeaponItemData>(TempItem->ItemData);
				if (WeaponData)
				{
					if (InventoryPawn->GetInventoryComponent()->PurchaseWeapon(WeaponData->ItemMaterialQuantity, WeaponData->ItemPurchasePrice))
					{
						// 구매 가능하고, 이미 지불했으므로 아이템 추가해주기
						int32 ResultItemQuantity = 0;
						InventoryPawn->GetInventoryComponent()->AddItem(*WeaponData->GetName(), 1, ResultItemQuantity);
					}
				}
			}
		}
	}
}

void AMMBlacksmithNPC::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->GetHUDWidget()->InteractionWidgetHelpText(HelpText);
			PlayerController->GetHUDWidget()->ToggleInteractionWidget(true);
		}
	}
}

void AMMBlacksmithNPC::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter)
	{
		// 플레이어 HUD 위젯에 접근하여 InteractionWidget을 비활성화 시켜줍니다.
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->GetHUDWidget()->ToggleInteractionWidget(false);
		}
	}
}

void AMMBlacksmithNPC::Interaction(ACharacter* PlayerCharacter)
{
	TempPlayer = PlayerCharacter;
	IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(PlayerCharacter);

	if (!InventoryPawn) return;

	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());

	if (!ShopWidget)
	{
		if (ShopWidgetClass)
		{
			ShopWidget = CreateWidget<UMMBlacksmithWidget>(GetWorld(), ShopWidgetClass);

			// 인벤토리의 OnChangeInven 델리게이트에 함수 연동
			InventoryPawn->GetInventoryComponent()->OnChangeInven.AddUObject(ShopWidget, &UMMBlacksmithWidget::UpdateInventorySlot);
			InventoryPawn->GetInventoryComponent()->OnChangeGold.AddUObject(ShopWidget, &UMMBlacksmithWidget::UpdateGold);

			// 상점 위젯 초기화
			ShopWidget->SetOwningActor(this);
			ShopWidget->Init(PlayerCharacter);
			ShopWidget->AddToViewport();
		}
	}
	else
	{
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayerController)
	{
		// UI모드로 설정
		FInputModeUIOnly InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;

		InventoryPawn->GetInventoryComponent()->SetSellable(true);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/MMShopNPC.h"
#include "Interface/MMInventoryInterface.h"
#include "Player/MMInventoryComponent.h"
#include "Player/MMPlayerController.h"
#include "UI/NPC/MMShopWidget.h"
#include "Player/MMInventoryItem.h"
#include "UI/MMHUDWidget.h"
#include "Item/MMItemData.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"

AMMShopNPC::AMMShopNPC()
{
	HelpText = TEXT("Press F to open the Shop.");
}

void AMMShopNPC::BeginPlay()
{
	Super::BeginPlay();
}

void AMMShopNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMMShopNPC::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AMMShopNPC::OnEndOverlap);

	// 아이템 생성
	for (const auto& ItemData : ShopItemData)
	{
		if (ItemData)
		{
			UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
			NewItem->ItemData = ItemData;
			NewItem->ItemQuantity = 1;

			ShopItems.Add(NewItem);
		}
	}
}

void AMMShopNPC::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AMMShopNPC::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AMMShopNPC::Interaction(ACharacter* PlayerCharacter)
{
	TempPlayer = PlayerCharacter;
	IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(PlayerCharacter);

	if (!InventoryPawn) return;

	AMMPlayerController* PlayerController = Cast<AMMPlayerController>(PlayerCharacter->GetController());

	if (!ShopWidget)
	{
		if (ShopWidgetClass)
		{
			ShopWidget = CreateWidget<UMMShopWidget>(GetWorld(), ShopWidgetClass);

			// 인벤토리의 OnChangeInven 델리게이트에 함수 연동
			InventoryPawn->GetInventoryComponent()->OnChangeInven.AddUObject(ShopWidget, &UMMShopWidget::UpdateInventorySlot);
			InventoryPawn->GetInventoryComponent()->OnChangeGold.AddUObject(ShopWidget, &UMMShopWidget::UpdateGold);
			
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

void AMMShopNPC::CloseShop()
{
	if (TempPlayer && ShopWidget)
	{
		// 게임모드로 전환
		AMMPlayerController* PlayerController = Cast<AMMPlayerController>(TempPlayer->GetController());
		if (PlayerController)
		{
			// 활성화된 위젯이 있다면?
			if (PlayerController->GetHUDWidget()->GetIsVisibility())
			{
				// UI & Game 모드로 전환
				PlayerController->SetUIInputMode();
			}
			else
			{
				// Game 모드로 전환
				PlayerController->SetGameInputMode();
			}
		}
		
		// 판매 기능 비활성화
		IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(TempPlayer);
		if (InventoryPawn)
		{
			InventoryPawn->GetInventoryComponent()->SetSellable(false);
		}
		
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		TempPlayer = nullptr;
	}
}

void AMMShopNPC::Purchase(int32 ItemIndex)
{
	if (TempPlayer)
	{
		IMMInventoryInterface* InventoryPawn = Cast<IMMInventoryInterface>(TempPlayer);
		if (InventoryPawn)
		{
			// 전달받은 인덱스가 정확한지 확인하기
			if (ShopItems.IsValidIndex(ItemIndex) && IsValid(ShopItems[ItemIndex]))
			{
				// 현재 아이템을 구매할 수 있다면?
				if (InventoryPawn->GetInventoryComponent()->UseGold(ShopItems[ItemIndex]->ItemData->ItemPurchasePrice))
				{
					int32 ResultItemQuantity = 0;
					FName ItemName = *ShopItems[ItemIndex]->ItemData->GetName();

					// 아이템 구매하기 (성공시 true, 실패시 false)
					if (!InventoryPawn->GetInventoryComponent()->AddItem(ItemName, 1, ResultItemQuantity))
					{
						// 아이템 구매에 실패한 경우 돈 복구하기
						InventoryPawn->GetInventoryComponent()->AddGold(ShopItems[ItemIndex]->ItemData->ItemPurchasePrice);
					}
				}
			}
		}
	}
}

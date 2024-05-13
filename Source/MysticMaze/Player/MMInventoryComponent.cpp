// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"
#include "Item/MMItemData.h"

#include "Engine/AssetManager.h"

UMMInventoryComponent::UMMInventoryComponent()
{
	bWantsInitializeComponent = true;

	MaxInventoryNum = 30;
	MaxItemNum = 99;

	// 초기 사이즈 지정
	EquipmentItems.Init(nullptr, MaxInventoryNum);
	ConsumableItems.Init(nullptr, MaxInventoryNum);
	OtherItems.Init(nullptr, MaxInventoryNum);
}

void UMMInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 인벤토리 초기화
	InitInventory();
}

void UMMInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UMMInventoryComponent::AddItem(FName InItemName, int32 InItemQuantity, int32& OutItemQuantity)
{
	// 성공적으로 추가했는지에 대한 결과 반환용 변수
	bool bIsResult = false;

	// 애셋 매니저 불러오기
	UAssetManager& Manager = UAssetManager::Get();

	// 애셋 아이디 리스트 받아오기
	TArray<FPrimaryAssetId> Assets;
	// * 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 반환받음
	Manager.GetPrimaryAssetIdList(TEXT("MMItemData"), Assets);

	// 특정 아이템 키 생성
	FPrimaryAssetId Key;
	Key.PrimaryAssetType = TEXT("MMItemData");
	Key.PrimaryAssetName = InItemName;

	// 해당 키의 애셋이 존재한다면?
	if (Assets.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Data"));

		// 아이템 생성
		UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
		if (NewItem)
		{
			FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets.FindByKey(Key)[0]));
			if (AssetPtr.IsPending())
			{
				AssetPtr.LoadSynchronous();
			}
			UMMItemData* ItemData = Cast<UMMItemData>(AssetPtr.Get());
			if (ItemData)
			{
				NewItem->ItemData = ItemData;
				NewItem->ItemQuantity = InItemQuantity;
			}
			else
			{
				return bIsResult;
			}
		}

		// 이미 해당 아이템이 존재하는지 체크하기 (포션, 기타)
		if (NewItem->ItemData->ItemType != EItemType::IT_Weapon)
		{
			// 타입에 따라 해당 인벤토리에 저장하기
			switch (NewItem->ItemData->ItemType)
			{
			case EItemType::IT_Potion:
				for (UMMInventoryItem* Item : ConsumableItems)
				{
					if (IsValid(Item))
					{
						// 이름이 동일한 아이템이 있는지 체크하기
						if (Item->ItemData->ItemName == NewItem->ItemData->ItemName)
						{
							// 최대 수량 체크하기
							if (Item->ItemQuantity + NewItem->ItemQuantity> MaxItemNum)
							{
								// 최대 수량까지 채운 후 남은 값으로 설정하기
								NewItem->ItemQuantity = Item->ItemQuantity + NewItem->ItemQuantity - MaxItemNum;
								Item->ItemQuantity = MaxItemNum;
								break;
							}
							else
							{
								// 수량을 더하고 종료하기
								Item->ItemQuantity += NewItem->ItemQuantity;
								OnChangeInven.Broadcast();
								return true;
							}
						}
					}
				}
				break;

			case EItemType::IT_ManaStone:
				for (UMMInventoryItem* Item : OtherItems)
				{
					if (IsValid(Item))
					{
						// 이름이 동일한 아이템이 있는지 체크하기
						if (Item->ItemData->ItemName == NewItem->ItemData->ItemName)
						{
							// 최대 수량 체크하기
							if (Item->ItemQuantity + NewItem->ItemQuantity > MaxItemNum)
							{
								// 최대 수량까지 채운 후 남은 값으로 설정하기
								NewItem->ItemQuantity = Item->ItemQuantity + NewItem->ItemQuantity - MaxItemNum;
								Item->ItemQuantity = MaxItemNum;
								break;
							}
							else
							{
								// 수량을 더하고 종료하기
								Item->ItemQuantity += NewItem->ItemQuantity;
								OnChangeInven.Broadcast();
								return true;
							}
						}
					}
				}
				break;
			}
		}

		// 해당 아이템 타입의 인벤토리의 빈 칸 찾아 데이터 추가하기
		int32 Index = 0;
		switch (NewItem->ItemData->ItemType)
		{
		case EItemType::IT_Weapon:
			for (UMMInventoryItem* Item : EquipmentItems)
			{
				if (!IsValid(Item))
				{
					OtherItems[Index] = NewItem;
					bIsResult = true;
					OnChangeInven.Broadcast();
					break;
				}

				Index++;
			}
			break;
		case EItemType::IT_Potion:
			for (UMMInventoryItem* Item : ConsumableItems)
			{
				if (!IsValid(Item))
				{
					OtherItems[Index] = NewItem;
					bIsResult = true;
					OnChangeInven.Broadcast();
					break;
				}

				Index++;
			}
			break;
		case EItemType::IT_ManaStone:
			for (UMMInventoryItem* Item : OtherItems)
			{
				if (!IsValid(Item))
				{
					OtherItems[Index] = NewItem;
					bIsResult = true;
					OnChangeInven.Broadcast();
					break;
				}

				Index++;
			}
			break;
		}

		if (!bIsResult)
		{
			OutItemQuantity = NewItem->ItemQuantity;
			return bIsResult;
		}
		else
		{
			return bIsResult;
		}
	}

	OutItemQuantity = InItemQuantity;
	return bIsResult;
}

void UMMInventoryComponent::AddGold(int32 InGold)
{
	if (InGold < 0) return;

	CurrentGold += InGold;
}

void UMMInventoryComponent::InitInventory()
{
	// TODO : 파일로부터 정보 읽어와서 설정하기
	CurrentGold = 1000;

	// 애셋 매니저 불러오기
	UAssetManager& Manager = UAssetManager::Get();

	// 애셋 아이디 리스트 받아오기
	TArray<FPrimaryAssetId> Assets;
	// * 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 반환받음
	Manager.GetPrimaryAssetIdList(TEXT("MMItemData"), Assets);

	if (Assets.Num() > 0)
	{
		TMap<int32, FName> InventoryWeaponArray;
		// TODO : 세이브 파일에서 데이터 읽어오기 (현재는 테스트 용도)
		{
			InventoryWeaponArray.Add(1, TEXT("DA_Staff_BluntBell"));
			InventoryWeaponArray.Add(5, TEXT("DA_Staff_BluntHellHammerCine"));
			InventoryWeaponArray.Add(3, TEXT("DA_Bow_2"));
		}

		for (const auto& InvItem : InventoryWeaponArray)
		{
			// 특정 아이템 키 생성
			FPrimaryAssetId Key;
			Key.PrimaryAssetType = TEXT("MMItemData");
			Key.PrimaryAssetName = InvItem.Value;

			if (Assets.Contains(Key))
			{
				// 아이템 생성
				UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
				if (NewItem)
				{
					FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets.FindByKey(Key)[0]));
					if (AssetPtr.IsPending())
					{
						AssetPtr.LoadSynchronous();
					}
					UMMItemData* ItemData = Cast<UMMItemData>(AssetPtr.Get());
					if (ItemData)
					{
						NewItem->ItemData = ItemData;
						NewItem->ItemQuantity = 1;
						// 아이템 넣기
						EquipmentItems[InvItem.Key] = NewItem;
					}
				}
			}
		}
	}
}


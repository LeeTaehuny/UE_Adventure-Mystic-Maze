// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MMInventoryComponent.h"
#include "Player/MMInventoryItem.h"
#include "Player/MMStatComponent.h"
#include "Item/MMWeapon.h"
#include "Item/MMItemData.h"
#include "Item/MMPotionItemData.h"
#include "Item/MMWeaponItemData.h"
#include "Interface/MMStatusInterface.h"
#include "Interface/MMInventoryInterface.h"
#include "Interface/MMPlayerClassInterface.h"
#include "GameData/MMStructures.h"
#include "Game/MMSaveGameData.h"
#include "Game/MMGameInstance.h"

#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

UMMInventoryComponent::UMMInventoryComponent()
{
	bWantsInitializeComponent = true;

	MaxInventoryNum = 30;
	MaxItemNum = 99;

	// 초기 사이즈 지정
	EquipmentItems.Init(nullptr, MaxInventoryNum);
	ConsumableItems.Init(nullptr, MaxInventoryNum);
	OtherItems.Init(nullptr, MaxInventoryNum);
	PotionQuickSlots.Init(nullptr, 2);

	// 판매 기능 설정
	bIsSellable = false;
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
							// 현재 칸이 최대 이미 최대 수량이라면 건너뛰기
							if (Item->ItemQuantity == MaxItemNum) continue;

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
								OnChangedPotionSlot.Broadcast();
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
							// 현재 칸이 최대 이미 최대 수량이라면 건너뛰기
							if (Item->ItemQuantity == MaxItemNum) continue;

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
					EquipmentItems[Index] = NewItem;
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
					ConsumableItems[Index] = NewItem;
					bIsResult = true;
					OnChangeInven.Broadcast();
					OnChangedPotionSlot.Broadcast();
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

void UMMInventoryComponent::UseItem(int32 InSlotIndex, ESlotType InventoryType)
{
	// 해당 인벤토리 슬롯에 아이템이 존재하는지 체크하고 사용하기
	switch (InventoryType)
	{
	case ESlotType::ST_PotionSlot:
		if (PotionQuickSlots.IsValidIndex(InSlotIndex) && IsValid(PotionQuickSlots[InSlotIndex]))
		{
			int32 ItemIndex = 0;
			int32 ConsumableItemIndex = -1;

			// 소비 슬롯에서 인덱스를 찾아줍니다.
			for (const auto& Item : ConsumableItems)
			{
				if (IsValid(ConsumableItems[ItemIndex]))
				{
					if (Item->ItemData == PotionQuickSlots[InSlotIndex]->ItemData)
					{
						ConsumableItemIndex = ItemIndex;
						break;
					}
				}

				ItemIndex++;
			}

			// 인덱스를 찾았다면?
			if (ConsumableItemIndex != -1)
			{
				if (ConsumableItems.IsValidIndex(ConsumableItemIndex) && IsValid(ConsumableItems[ConsumableItemIndex]))
				{
					if (ConsumableItems[ConsumableItemIndex]->ItemQuantity >= 1)
					{
						// 아이템이 포션인지 체크합니다.
						UMMPotionItemData* PotionData = Cast<UMMPotionItemData>(ConsumableItems[ConsumableItemIndex]->ItemData);
						IMMStatusInterface* StatusPawn = Cast<IMMStatusInterface>(GetOwner());
						if (PotionData && StatusPawn)
						{
							// 포션 타입에 따라 회복시켜주도록 합니다.
							switch (PotionData->PotionType)
							{
							case EPotionType::PT_Hp:
								StatusPawn->GetStatComponent()->HealHp(PotionData->Percent);
								break;

							case EPotionType::PT_Mp:
								StatusPawn->GetStatComponent()->HealMp(PotionData->Percent);
								break;
							}

							// 수량을 줄여줍니다.
							ConsumableItems[ConsumableItemIndex]->ItemQuantity--;
						}
					}

					if (ConsumableItems[ConsumableItemIndex]->ItemQuantity <= 0)
					{
						RemoveItem(ConsumableItemIndex, ESlotType::ST_InventoryConsumable);
					}
				}
			}

			OnChangeInven.Broadcast();
			OnChangedPotionSlot.Broadcast();
		}
		break;
	}
}

void UMMInventoryComponent::SellItem(int32 InSlotIndex, ESlotType InventoryType)
{
	if (!bIsSellable) return;

	switch (InventoryType)
	{
	case ESlotType::ST_InventoryEquipment:
		if (EquipmentItems.IsValidIndex(InSlotIndex) && IsValid(EquipmentItems[InSlotIndex]))
		{
			// 수량을 줄여줍니다.
			EquipmentItems[InSlotIndex]->ItemQuantity--;
			// 골드를 추가합니다.
			AddGold(EquipmentItems[InSlotIndex]->ItemData->ItemSalePrice);

			// 수량이 0 이하라면 소멸시켜줍니다.
			if (EquipmentItems[InSlotIndex]->ItemQuantity <= 0)
			{
				RemoveItem(InSlotIndex, InventoryType);
			}

			OnChangeInven.Broadcast();
		}
		break;
	case ESlotType::ST_InventoryConsumable:
		if (ConsumableItems.IsValidIndex(InSlotIndex) && IsValid(ConsumableItems[InSlotIndex]))
		{
			// 수량을 줄여줍니다.
			ConsumableItems[InSlotIndex]->ItemQuantity--;
			// 골드를 추가합니다.
			AddGold(ConsumableItems[InSlotIndex]->ItemData->ItemSalePrice);

			// 수량이 0 이하라면 소멸시켜줍니다.
			if (ConsumableItems[InSlotIndex]->ItemQuantity <= 0)
			{
				RemoveItem(InSlotIndex, InventoryType);
			}

			OnChangeInven.Broadcast();
			OnChangedPotionSlot.Broadcast();
		}
		break;
	case ESlotType::ST_InventoryOther:
		if (OtherItems.IsValidIndex(InSlotIndex) && IsValid(OtherItems[InSlotIndex]))
		{
			// 수량을 줄여줍니다.
			OtherItems[InSlotIndex]->ItemQuantity--;
			// 골드를 추가합니다.
			AddGold(OtherItems[InSlotIndex]->ItemData->ItemSalePrice);

			// 수량이 0 이하라면 소멸시켜줍니다.
			if (OtherItems[InSlotIndex]->ItemQuantity <= 0)
			{
				RemoveItem(InSlotIndex, InventoryType);
			}

			OnChangeInven.Broadcast();
		}
		break;
	}
}

bool UMMInventoryComponent::PurchaseWeapon(int32 InQuantity, int32 InPrice)
{
	bool Result = false;

	// 가격 확인 (가격이 비싼 경우 반환)
	if (InPrice > CurrentGold) return Result;

	// 빈 슬롯이 있는지 확인
	bool bIsFull = true;
	for (const auto& Weapon : EquipmentItems)
	{
		if (!IsValid(Weapon))
		{
			// 빈 칸이 있으므로 가득 차있지 않다고 표시
			bIsFull = false;
			break;
		}
	}

	// 빈 슬롯이 없으면 반환
	if (bIsFull)
	{
		return Result;
	}

	// 필요 마석 수 확인
	int32 TempNum = InQuantity;
	// 임시 배열
	TArray<int32> DelItemIndex;

	int32 Index = 0;
	for (UMMInventoryItem* Item : OtherItems)
	{
		if (IsValid(Item))
		{
			// 마나스톤인 경우
			if (Item->ItemData->ItemName == TEXT("마나스톤"))
			{
				// 수량이 충분하다면?
				if (Item->ItemQuantity >= TempNum)
				{
					// 제거하고 true 리턴하기
					Item->ItemQuantity -= TempNum;
					
					// 제거 후 남은 개수가 0개라면?
					if (Item->ItemQuantity == 0)
					{
						// 아이템 제거
						RemoveItem(Index, ESlotType::ST_InventoryOther);
					}

					// 결과 참으로 바꾸고 반복문 종료
					Result = true;
					break;
				}
				// 수량이 충분하지 못하다면?
				else if (Item->ItemQuantity < TempNum)
				{
					// 해당 인덱스를 제거 인덱스로 추가하기
					DelItemIndex.Add(Index);
					// 수량 줄이기
					TempNum -= Item->ItemQuantity;
				}
			}
		}

		Index++;
	}

	// 만약 아이템 제거에 성공했다면?
	if (Result)
	{
		// 제거 인덱스에 있는 모든 아이템 제거하기
		for (int32 i : DelItemIndex)
			RemoveItem(i, ESlotType::ST_InventoryOther);

		// 돈 소모하기
		CurrentGold -= InPrice;

		// 이벤트 발송 (인벤토리 및 돈)
		OnChangeInven.Broadcast();
		OnChangeGold.Broadcast();
	}

	return Result;
}

void UMMInventoryComponent::AddGold(int32 InGold)
{
	if (InGold < 0) return;

	CurrentGold += InGold;
	OnChangeGold.Broadcast();
}

bool UMMInventoryComponent::UseGold(int32 InGold)
{
	if (InGold < 0) return false;

	// 금액 소모가 가능한 상황
	if (CurrentGold - InGold >= 0)
	{
		// 금액 소모 및 이벤트 발생
		CurrentGold -= InGold;
		OnChangeGold.Broadcast();
		return true;
	}
	// 금액 소모가 불가능한 상황
	else
	{
		return false;
	}
}

void UMMInventoryComponent::SwapItem(int32 InPrevIndex, int32 InCurrentIndex, ESlotType InPrevSlotType, ESlotType InCurrentSlotType)
{
	// 슬롯 타입이 같은 경우 교환해주도록 합니다.
	if (InPrevSlotType == InCurrentSlotType)
	{
		switch (InCurrentSlotType)
		{
		case ESlotType::ST_InventoryEquipment:
			// 해당 슬롯의 아이템이 유효한지 체크합니다.
			if (EquipmentItems.IsValidIndex(InPrevIndex) && EquipmentItems.IsValidIndex(InCurrentIndex))
			{
				// 교체 후 이벤트를 호출합니다.
				EquipmentItems.Swap(InPrevIndex, InCurrentIndex);
				OnChangeInven.Broadcast();
			}
			break;

		case ESlotType::ST_InventoryConsumable:
			// 해당 슬롯의 아이템이 유효한지 체크합니다.
			if (ConsumableItems.IsValidIndex(InPrevIndex) && ConsumableItems.IsValidIndex(InCurrentIndex))
			{
				// 교체 후 이벤트를 호출합니다.
				ConsumableItems.Swap(InPrevIndex, InCurrentIndex);
				OnChangeInven.Broadcast();
			}
			break;

		case ESlotType::ST_InventoryOther:
			// 해당 슬롯의 아이템이 유효한지 체크합니다.
			if (OtherItems.IsValidIndex(InPrevIndex) && OtherItems.IsValidIndex(InCurrentIndex))
			{
				// 교체 후 이벤트를 호출합니다.
				OtherItems.Swap(InPrevIndex, InCurrentIndex);
				OnChangeInven.Broadcast();
			}
			break;
		}
	}
}

void UMMInventoryComponent::SortItem(ESlotType InSlotType)
{
	// 슬롯 타입에 따라 정렬합니다.
	switch (InSlotType)
	{
	case ESlotType::ST_InventoryEquipment:
		Algo::Sort(EquipmentItems,
			[](const TObjectPtr<UMMInventoryItem>& A, const TObjectPtr<UMMInventoryItem>& B)
			{
				// 해당 요소가 nullptr이라면 뒤로 배치합니다
				if (!IsValid(A) && !IsValid(B))
					return false;
				else if (!IsValid(A))
					return false;
				else if (!IsValid(B))
					return true;

				// 동일한 이름의 아이템이라면 수량 순으로 배치합니다.
				if (A->ItemData->ItemName == B->ItemData->ItemName)
				{
					return A->ItemQuantity > B->ItemQuantity;
				}
				// 다른 이름의 아이템이라면 아이템 이름 순으로 배치합니다.
				else
				{
					return A->ItemData->ItemName < B->ItemData->ItemName;
				}
			}
		);
		OnChangeInven.Broadcast();
		break;
	
	case ESlotType::ST_InventoryConsumable:
		Algo::Sort(ConsumableItems,
			[](const TObjectPtr<UMMInventoryItem>& A, const TObjectPtr<UMMInventoryItem>& B)
			{
				// 해당 요소가 nullptr이라면 뒤로 배치합니다
				if (!IsValid(A) && !IsValid(B))
					return false;
				else if (!IsValid(A))
					return false;
				else if (!IsValid(B))
					return true;

				// 동일한 이름의 아이템이라면 수량 순으로 배치합니다.
				if (A->ItemData->ItemName == B->ItemData->ItemName)
				{
					return A->ItemQuantity > B->ItemQuantity;
				}
				// 다른 이름의 아이템이라면 아이템 이름 순으로 배치합니다.
				else
				{
					return A->ItemData->ItemName < B->ItemData->ItemName;
				}
			}
		);
		OnChangeInven.Broadcast();
		break;
	
	case ESlotType::ST_InventoryOther:
		Algo::Sort(OtherItems,
			[](const TObjectPtr<UMMInventoryItem>& A, const TObjectPtr<UMMInventoryItem>& B)
			{
				// 해당 요소가 nullptr이라면 뒤로 배치합니다
				if (!IsValid(A) && !IsValid(B))
					return false;
				else if (!IsValid(A))
					return false;
				else if (!IsValid(B))
					return true;

				// 동일한 이름의 아이템이라면 수량 순으로 배치합니다.
				if (A->ItemData->ItemName == B->ItemData->ItemName)
				{
					return A->ItemQuantity > B->ItemQuantity;
				}
				// 다른 이름의 아이템이라면 아이템 이름 순으로 배치합니다.
				else
				{
					return A->ItemData->ItemName < B->ItemData->ItemName;
				}
			}
		);
		OnChangeInven.Broadcast();
		break;
	}
}

void UMMInventoryComponent::SetQuickSlot(ESlotType InPrevSlotType, int32 InPrevIndex, int32 InCurrentIndex)
{
	if (InPrevSlotType == ESlotType::ST_InventoryConsumable)
	{
		if (ConsumableItems.IsValidIndex(InPrevIndex) && IsValid(ConsumableItems[InPrevIndex]) && PotionQuickSlots.IsValidIndex(InCurrentIndex))
		{
			PotionQuickSlots[InCurrentIndex] = ConsumableItems[InPrevIndex];
			OnChangedPotionSlot.Broadcast();
		}
	}
}

void UMMInventoryComponent::EquipItem(int32 InCurrentIndex)
{
	// 인덱스와 내부 데이터가 유효한지 체크합니다. 
	if (!EquipmentItems.IsValidIndex(InCurrentIndex) || !IsValid(EquipmentItems[InCurrentIndex])) return;

	// 장비 타입을 체크합니다.
	UMMWeaponItemData* WeaponData = Cast<UMMWeaponItemData>(EquipmentItems[InCurrentIndex]->ItemData);
	if (!WeaponData) return;
	IMMPlayerClassInterface* ClassPawn = Cast<IMMPlayerClassInterface>(GetOwner());
	if (!ClassPawn) return;

	// 착용할 수 없는 타입의 직업이라면 반환합니다.
	if (ClassPawn->GetClass() != WeaponData->PurchaseableClass) return;

	// 현재 착용중인 장비가 존재하는 경우
	if (IsValid(EquipmentItem))
	{
		// 장비를 교체합니다.
		UMMInventoryItem* TempItem = EquipmentItem;
		EquipmentItem = EquipmentItems[InCurrentIndex];
		EquipmentItems[InCurrentIndex] = TempItem;
	}
	else
	{
		// 새로운 장비를 장비 슬롯에 적용합니다.
		EquipmentItem = EquipmentItems[InCurrentIndex];
		EquipmentItems[InCurrentIndex] = nullptr;
	}

	// 장비를 착용합니다.
	IMMInventoryInterface* InvPawn = Cast<IMMInventoryInterface>(GetOwner());
	if (InvPawn)
	{
		InvPawn->EquipWeapon(Cast<AMMWeapon>(GetWorld()->SpawnActor<AMMWeapon>(WeaponData->WeaponClass)));
	}

	// 장비의 스탯을 적용합니다.
	IMMStatusInterface* StatusPawn = Cast<IMMStatusInterface>(GetOwner());
	if (StatusPawn)
	{
		StatusPawn->GetStatComponent()->SetWeaponStat(WeaponData->WeaponStat);
	}
	
	// 인벤토리 변경 이벤트 발생
	OnChangeInven.Broadcast();
}

void UMMInventoryComponent::UnEquipItem()
{
	// 결과 반환용 변수
	bool Result = false;

	// 현재 착용중인 장비가 유효한지 체크합니다.
	if (!IsValid(EquipmentItem)) return;

	// 현재 장비를 장비 창의 빈 칸에 넣어줍니다.
	for (auto& Item : EquipmentItems)
	{
		if (!IsValid(Item))
		{
			Item = EquipmentItem;
			EquipmentItem = nullptr;
			Result = true;
			break;
		}
	}

	// 착용 해제에 성공한 경우
	if (Result)
	{
		// 장비 스탯을 초기화합니다.
		IMMStatusInterface* StatusPawn = Cast<IMMStatusInterface>(GetOwner());
		if (StatusPawn)
		{
			StatusPawn->GetStatComponent()->SetWeaponStat();
		}

		// 실제 장비를 착용해제 합니다.
		IMMInventoryInterface* InvPawn = Cast<IMMInventoryInterface>(GetOwner());
		if (InvPawn)
		{
			InvPawn->UnEquipWeapon();
		}

		// 인벤토리 변경 이벤트 발생
		OnChangeInven.Broadcast();
	}

	return;
}

void UMMInventoryComponent::SaveInventory()
{
	// GameInstance에서 Save파일 이름 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	// 게임 데이터 인스턴스 받아오기
	UMMSaveGameData* GameData = GameInstance->GetSaveData();
	if (GameData)
	{
		// 현재 골드 저장
		GameData->Gold = CurrentGold;

		// 장비슬롯 저장
		{
			TArray<FMMItemSaveData> InventoryEquipment;
			for (int i = 0; i < EquipmentItems.Num(); i++)
			{
				// 해당 슬롯에 아이템이 존재하는 경우
				if (IsValid(EquipmentItems[i]))
				{
					// 아이템 정보를 저장합니다.
					FMMItemSaveData ItemSaveData;

					ItemSaveData.SlotIndex = i;
					ItemSaveData.ItemName = *EquipmentItems[i]->ItemData->GetName();
					ItemSaveData.ItemQuantity = 1;

					// 아이템 정보를 저장합니다.
					InventoryEquipment.Add(ItemSaveData);
				}
			}

			GameData->InventoryEquipmentArray = InventoryEquipment;
		}

		// 소비슬롯 저장
		{
			TArray<FMMItemSaveData> InventoryConstable;
			for (int i = 0; i < ConsumableItems.Num(); i++)
			{
				// 해당 슬롯에 아이템이 존재하는 경우
				if (IsValid(ConsumableItems[i]))
				{
					// 아이템 정보를 저장합니다.
					FMMItemSaveData ItemSaveData;

					ItemSaveData.SlotIndex = i;
					ItemSaveData.ItemName = *ConsumableItems[i]->ItemData->GetName();
					ItemSaveData.ItemQuantity = ConsumableItems[i]->ItemQuantity;

					// 아이템 정보를 저장합니다.
					InventoryConstable.Add(ItemSaveData);
				}
			}

			GameData->InventoryConstableArray = InventoryConstable;
		}

		// 기타슬롯 저장
		{
			TArray<FMMItemSaveData> InventoryOther;
			for (int i = 0; i < OtherItems.Num(); i++)
			{
				// 해당 슬롯에 아이템이 존재하는 경우
				if (IsValid(OtherItems[i]))
				{
					// 아이템 정보를 저장합니다.
					FMMItemSaveData ItemSaveData;

					ItemSaveData.SlotIndex = i;
					ItemSaveData.ItemName = *OtherItems[i]->ItemData->GetName();
					ItemSaveData.ItemQuantity = OtherItems[i]->ItemQuantity;

					// 아이템 정보를 저장합니다.
					InventoryOther.Add(ItemSaveData);
				}
			}

			GameData->InventoryOtherArray = InventoryOther;
		}

		// 현재 장착 무기 저장
		{
			if (IsValid(EquipmentItem))
			{
				GameData->EquipmentWeapon = *EquipmentItem->ItemData->GetName();
			}
			else
			{
				GameData->EquipmentWeapon = TEXT("");
			}
		}
		
		// 퀵슬롯 포션 인덱스 저장
		{
			TArray<int32> PotionSlotIndexs;
			for (int i = 0; i < PotionQuickSlots.Num(); i++)
			{
				// 장착된 포션이 있다면?
				if (IsValid(PotionQuickSlots[i]))
				{
					// 소비슬롯을 순회하며 해당 포션 찾기
					for (int j = 0; j < ConsumableItems.Num(); j++)
					{
						if (IsValid(ConsumableItems[j]))
						{
							// 동일한 경우 해당 인덱스 저장
							if (PotionQuickSlots[i] == ConsumableItems[j])
							{
								PotionSlotIndexs.Add(j);
							}
						}
					}
				}
			}

			GameData->PotionSlotIndexs = PotionSlotIndexs;
		}

		// 게임 저장하기
		UGameplayStatics::SaveGameToSlot(GameData, GameData->SaveSlotName, GameData->SaveIndex);
	}
}

void UMMInventoryComponent::InitInventory()
{
	// GameInstance에서 Save파일 받아오기
	UMMGameInstance* GameInstance = Cast<UMMGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) return;

	UMMSaveGameData* GameData = GameInstance->GetSaveData();
	// 저장된 게임 데이터가 있는 경우라면?
	if (GameData)
	{
		// 골드 설정
		CurrentGold = GameData->Gold;

		// 애셋 매니저 불러오기
		UAssetManager& Manager = UAssetManager::Get();

		// 애셋 아이디 리스트 받아오기
		TArray<FPrimaryAssetId> Assets;
		// * 태그 정보를 넘겨줘서 동일한 태그를 가진 애셋들의 목록을 배열로 반환받음
		Manager.GetPrimaryAssetIdList(TEXT("MMItemData"), Assets);

		if (Assets.Num() > 0)
		{
			TArray<FMMItemSaveData> InventoryEquipment = GameData->InventoryEquipmentArray;
			TArray<FMMItemSaveData> InventoryConstable = GameData->InventoryConstableArray;
			TArray<FMMItemSaveData> InventoryOther = GameData->InventoryOtherArray;

			for (const auto& InvItem : InventoryEquipment)
			{
				// 특정 아이템 키 생성
				FPrimaryAssetId Key;
				Key.PrimaryAssetType = TEXT("MMItemData");
				Key.PrimaryAssetName = InvItem.ItemName;

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
							NewItem->ItemQuantity = InvItem.ItemQuantity;
							// 아이템 넣기
							EquipmentItems[InvItem.SlotIndex] = NewItem;
						}
					}
				}
			}

			for (const auto& InvItem : InventoryConstable)
			{
				// 특정 아이템 키 생성
				FPrimaryAssetId Key;
				Key.PrimaryAssetType = TEXT("MMItemData");
				Key.PrimaryAssetName = InvItem.ItemName;

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
							NewItem->ItemQuantity = InvItem.ItemQuantity;
							// 아이템 넣기
							ConsumableItems[InvItem.SlotIndex] = NewItem;
						}
					}
				}
			}

			for (const auto& InvItem : InventoryOther)
			{
				// 특정 아이템 키 생성
				FPrimaryAssetId Key;
				Key.PrimaryAssetType = TEXT("MMItemData");
				Key.PrimaryAssetName = InvItem.ItemName;

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
							NewItem->ItemQuantity = InvItem.ItemQuantity;
							// 아이템 넣기
							OtherItems[InvItem.SlotIndex] = NewItem;
						}
					}
				}
			}

			// 기존 장착 무기 착용
			FName EquipmentWeapon = GameData->EquipmentWeapon;
			{
				FPrimaryAssetId Key;
				Key.PrimaryAssetType = TEXT("MMItemData");
				Key.PrimaryAssetName = EquipmentWeapon;

				if (Assets.Contains(Key))
				{
					UMMInventoryItem* NewItem = NewObject<UMMInventoryItem>();
					if (NewItem)
					{
						FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets.FindByKey(Key)[0]));
						if (AssetPtr.IsPending())
						{
							AssetPtr.LoadSynchronous();
						}
						UMMItemData* WeaponData = Cast<UMMItemData>(AssetPtr.Get());
						if (WeaponData)
						{
							NewItem->ItemData = WeaponData;
							NewItem->ItemQuantity = 1;
							// 아이템 넣기
							EquipmentItem = NewItem;
						}
					}
				}

				if (IsValid(EquipmentItem))
				{
					// 장비 데이터를 읽어옵니다.
					UMMWeaponItemData* WeaponData = Cast<UMMWeaponItemData>(EquipmentItem->ItemData);
					if (WeaponData)
					{
						// 장비를 착용합니다.
						IMMInventoryInterface* InvPawn = Cast<IMMInventoryInterface>(GetOwner());
						if (InvPawn)
						{
							InvPawn->EquipWeapon(Cast<AMMWeapon>(GetWorld()->SpawnActor<AMMWeapon>(WeaponData->WeaponClass)));
						}

						// 장비의 스탯을 적용합니다.
						IMMStatusInterface* StatusPawn = Cast<IMMStatusInterface>(GetOwner());
						if (StatusPawn)
						{
							StatusPawn->GetStatComponent()->SetWeaponStat(WeaponData->WeaponStat);
						}
					}
				}
			}

			// 기존 퀵슬롯 등록 아이템 넣어주기
			TArray<int32> PotionSlotIndexs = GameData->PotionSlotIndexs;
			{
				for (int i = 0; i < PotionSlotIndexs.Num(); i++)
				{
					if (ConsumableItems.IsValidIndex(PotionSlotIndexs[i]) && IsValid(ConsumableItems[PotionSlotIndexs[i]]) && PotionQuickSlots.IsValidIndex(i))
					{
						PotionQuickSlots[i] = ConsumableItems[PotionSlotIndexs[i]];
						OnChangedPotionSlot.Broadcast();
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Load"));
	}
}

void UMMInventoryComponent::RemoveItem(int32 InSlotIndex, ESlotType InventoryType)
{
	// 해당 인벤토리 슬롯의 유효성을 체크하고 소멸시켜줍니다.
	switch (InventoryType)
	{
	case ESlotType::ST_InventoryEquipment:
		if (EquipmentItems.IsValidIndex(InSlotIndex) && IsValid(EquipmentItems[InSlotIndex]))
		{
			EquipmentItems[InSlotIndex] = nullptr;
		}
		break;

	case ESlotType::ST_InventoryConsumable:
		if (ConsumableItems.IsValidIndex(InSlotIndex) && IsValid(ConsumableItems[InSlotIndex]))
		{
			ConsumableItems[InSlotIndex] = nullptr;
		}
		break;

	case ESlotType::ST_InventoryOther:
		if (OtherItems.IsValidIndex(InSlotIndex) && IsValid(OtherItems[InSlotIndex]))
		{
			OtherItems[InSlotIndex] = nullptr;
		}
		break;
	}
}

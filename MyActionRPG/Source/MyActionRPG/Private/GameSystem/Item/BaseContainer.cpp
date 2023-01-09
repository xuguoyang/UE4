// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Item/BaseContainer.h"
#include "GameSystem/Item/BaseItem.h"
#include "GameSystem/Item/Component/GridContainerComponent.h"

void UBaseContainer::InitContainer(class UGridContainerComponent* grid)
{
	GridComponent = grid;

	if (Items.Num() != ItemMaxAmount)
	{
		Items.SetNumZeroed(ItemMaxAmount);
	}

	InitItemOwner();
}

void UBaseContainer::InitItemOwner()
{
	if (Items.Num() > 0)
	{
		for (UBaseItem* anItem : Items)
		{
			if (nullptr != anItem)
			{
				anItem->OwnerContainer = this;
			}
		}
	}
}

AActor* UBaseContainer::GetOwner()
{
	if (GridComponent.Get())
	{
		return GridComponent.Get()->GetOwner();
	}

	return nullptr;
}

class UBaseItem* UBaseContainer::CreateItemByClass(TSubclassOf<class UBaseItem> ItemSubclass, int32 AmountOverride /*= 1*/)
{
	if (nullptr == ItemSubclass)
	{
		return nullptr;
	}

	if (nullptr == GetOwner())
	{
		return nullptr;
	}

	UBaseItem* ItemCDO = ItemSubclass->GetDefaultObject<UBaseItem>();
	if (nullptr == ItemCDO)
	{
		return nullptr;
	}

	if (ItemCDO->MaxAmount <= 0)
	{
		return nullptr;
	}

	UBaseItem* newItem = NewObject<UBaseItem>(GetOwner(), ItemSubclass);
	if (nullptr == newItem)
	{
		return nullptr;
	}

	if (newItem->MaxAmount <= 0)
	{
		return nullptr;
	}

	if (AmountOverride > newItem->MaxAmount || AmountOverride <= 0)
	{
		return nullptr;
	}

	newItem->Amount = AmountOverride;

	return newItem;
}

bool UBaseContainer::AddItemByClass(TSubclassOf<class UBaseItem> ItemSubclass, EItemChangeReason ItemChangeReason, int32 AmountOverride /*= 1*/)
{
	UBaseItem* newItem = CreateItemByClass(ItemSubclass, 1);
	if (nullptr == newItem)
	{
		return false;
	}

	newItem->UniqueId = FGuid::NewGuid();
	newItem->AddItemReason = ItemChangeReason;

	// 增加多于最大数量
	int32 NeedAddAmount = 0;
	if (AmountOverride > newItem->MaxAmount)
	{
		// 最多MaxAmount的10倍
		if (AmountOverride > newItem->MaxAmount * MaxAddGroup)
		{
			AmountOverride = newItem->MaxAmount * MaxAddGroup;
		}

		NeedAddAmount = AmountOverride - newItem->MaxAmount;
		newItem->Amount = newItem->MaxAmount;
	}
	else
	{
		newItem->Amount = AmountOverride;
	}

	bool bShowNotification = false;
	bool bSuccessAddItem = false;
	bSuccessAddItem = AddItemToInExactSlot(newItem, bShowNotification);

	// 第一次不成功，就没必要递归了
	if (bSuccessAddItem && NeedAddAmount > 0)
	{
		AddItemByClass(ItemSubclass, ItemChangeReason, NeedAddAmount);
	}

	return bSuccessAddItem;
}

bool UBaseContainer::AddItemByClassSimple(TSubclassOf<class UBaseItem> ItemSubclass, EItemChangeReason ItemChangeReason, int32 AmountOverride /*= 1*/)
{
	return AddItemByClass(ItemSubclass, ItemChangeReason, AmountOverride);
}

bool UBaseContainer::AddItemByItemId(int32 ItemId, EItemChangeReason ItemChangeReason, int32 AmountOverride /*= 1*/)
{
	//UMUGameData* GameData = UMUGameData::GetGameData();
	//if (nullptr == GameData)
	//{
	//	//UE_LOG(LogMU, Warning, TEXT("UMUInventoryComponent::CreateItemByClass Function Failed, GameData is null Inventory = %s"), *GetOwnerDescriptionName());
	//	return false;
	//}

	TSubclassOf<class UBaseItem> ItemSubclass;// = GameData->GetItemClassByIndex(ItemId);
	if (ItemSubclass)
	{
		return AddItemByClass(ItemSubclass, ItemChangeReason, AmountOverride);
	}

	return false;
}

bool UBaseContainer::AddItemByNetInfo(const FItemNetInfo& ItemNetInfo, bool bShowNotification /*= false*/)
{
	if (!GridComponent.IsValid())
	{
		return false;
	}

	UBaseItem* newItem = UBaseItem::CreateItemFromNetInfo(ItemNetInfo, GetOwner());
	if (nullptr == newItem)
	{
		return false;
	}

	if (newItem->Amount <= 0)
	{
		return false;
	}

	if (GridComponent.Get()->GetLocalRole() == ROLE_Authority)
	{
		if (newItem->SlotIndex >= 0)
		{
			return AddItemToExactSlot(newItem, bShowNotification);
		}
		else
		{
			return AddItemToInExactSlot(newItem, bShowNotification);
		}
	}
	else
	{
		if (newItem->SlotIndex >= 0)
		{
			return AddItemToSlot(newItem, newItem->AddItemReason, bShowNotification);
		}
		else
		{
			return false;
		}
	}
}

bool UBaseContainer::AddItemToExactSlot(class UBaseItem* newItem, bool bShowNotification /*= false*/)
{
	if (nullptr == newItem)
	{
		return false;
	}

	if (!Items.IsValidIndex(newItem->SlotIndex))
	{
		return false;
	}

	UBaseItem* findItem = Items[newItem->SlotIndex];
	if (nullptr == findItem)
	{
		//该位置无道具
		return AddItemToSlot(newItem, newItem->AddItemReason, bShowNotification);
	}
	else
	{
		//该位置有道具
		if (newItem->CanMerge(findItem))
		{
			if (findItem->Amount + newItem->Amount <= findItem->MaxAmount)
			{
				findItem->SetAmount(findItem->Amount + newItem->Amount, newItem->AddItemReason, bShowNotification);
				return true;
			}
			else
			{
				//堆叠后总量大于物品最大堆叠数量 放入固定Slot内超过堆叠上限无法拾取？？
				return false;
			}
		}
	}

	return false;
}

bool UBaseContainer::AddItemToInExactSlot(class UBaseItem* newItem, bool bShowNotification /*= false*/)
{
	if (nullptr == newItem)
	{
		return false;
	}

	newItem->SlotIndex = -1;
	int32 ItemAmount = newItem->Amount;

	//道具可堆叠
	if (newItem->MaxAmount > 1)
	{
		if (MergeItem(newItem, newItem->Amount, newItem->AddItemReason, bShowNotification) == 0)
		{
			return true;
		}
		else
		{
			if (!CheckEmptySlot(newItem))
			{
				return false;
			}
		}
	}
	else
	{
		if (!CheckEmptySlot(newItem))
		{
			return false;
		}
	}
	return AddItemToSlot(newItem, newItem->AddItemReason, bShowNotification);
}

bool UBaseContainer::AddItemToSlot(class UBaseItem* newItem, EItemChangeReason ItemChangeReason, bool bShowNotification /*= false*/)
{
	if (nullptr == newItem)
	{
		return false;
	}

	if (newItem->Amount <= 0)
	{
		return false;
	}

	if (!Items.IsValidIndex(newItem->SlotIndex))
	{
		return false;
	}

	Items[newItem->SlotIndex] = newItem;
	newItem->Rename(NULL, GetOwner());

	if (newItem->OwnerContainer != this)
	{
		newItem->OwnerContainer = this;
	}

	if (GridComponent.Get())
	{
		GridComponent.Get()->NotifyClientItemStatus(ContainerType, ENotifyItemType::NOTIFY_ITEM_ADD, newItem, -1, bShowNotification);
		GridComponent.Get()->OnAddItem.Broadcast(this, ContainerType, newItem->SlotIndex);
	}

	return true;
}

bool UBaseContainer::TestAddItemToSlot(class UBaseItem* newItem, int32 SlotIndex)
{
	if (nullptr == newItem)
	{
		return false;
	}

	if (newItem->Amount <= 0)
	{
		return false;
	}

	if (!Items.IsValidIndex(SlotIndex))
	{
		return false;
	}

	return true;
}

int32 UBaseContainer::MergeItem(class UBaseItem* NeedMergeItem, int32 MergeAmount, EItemChangeReason ItemChangeReason, bool bShowNotification /*= false*/)
{
	if (NeedMergeItem->Amount < MergeAmount)
	{
		MergeAmount = NeedMergeItem->Amount;
	}

	if (NeedMergeItem->Amount <= 0)
	{
		return MergeAmount;
	}

	int32 LoopNum = 0;
	int32 StartIndex = 0;
	while (UBaseItem* CanMergeItem = GetCanMergeItem(NeedMergeItem, StartIndex))
	{
		if (MergeAmount <= 0)
		{
			return MergeAmount;
		}

		int32 CanAddAmount = CanMergeItem->MaxAmount - CanMergeItem->Amount;
		if (MergeAmount > CanAddAmount)
		{
			if (CanMergeItem->SetAmount(CanMergeItem->MaxAmount, ItemChangeReason, bShowNotification))
			{
			}
			MergeAmount = MergeAmount - CanAddAmount;
			if (NeedMergeItem->SetAmount(NeedMergeItem->Amount - CanAddAmount, ItemChangeReason, bShowNotification))
			{
			}
		}
		else
		{
			if (CanMergeItem->SetAmount(CanMergeItem->Amount + MergeAmount, ItemChangeReason, bShowNotification))
			{
			}

			if (NeedMergeItem->SetAmount(NeedMergeItem->Amount - MergeAmount, ItemChangeReason, bShowNotification))
			{
			}

			MergeAmount = 0;

			return MergeAmount;
		}

		LoopNum++;
		if (LoopNum > 10)
		{
			return MergeAmount;
		}
	}

	return MergeAmount;
}

class UBaseItem* UBaseContainer::GetCanMergeItem(class UBaseItem* newItem, int32& StartIndex)
{
	if (nullptr == newItem)
	{
		return nullptr;
	}

	if (StartIndex >= Items.Num())
	{
		return nullptr;
	}

	for (size_t i = StartIndex; i < Items.Num(); i++)
	{
		if (Items[i] == nullptr)
		{
			continue;
		}

		if (Items[i] == newItem)
		{
			continue;
		}

		if (newItem->CanMerge(Items[i]))
		{
			StartIndex = i;
			return Items[i];
		}
	}

	return nullptr;
}

bool UBaseContainer::CheckEmptySlot(class UBaseItem* newItem)
{
	if (nullptr == newItem)
	{
		return false;
	}

	int32 slot = GetFirstEmptySlot();
	if (slot < 0)
	{
		return false;
	}

	newItem->SlotIndex = slot;

	return true;
}

int32 UBaseContainer::GetFirstEmptySlot()
{
	int slot = 0;
	for (UBaseItem* anItem : Items)
	{
		if (anItem == NULL)
		{
			return slot;
		}
		slot++;
	}

	return INDEX_NONE;
}

bool UBaseContainer::RemoveItemByUniqueId(const FGuid& TheUniqueId, EItemChangeReason ItemChangeReason, int32 Amount /*= -1*/, bool bShowNotification /*= false */)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i] && Items[i]->UniqueId == TheUniqueId)
		{
			return RemoveItemBySlot(i, ItemChangeReason, Amount, bShowNotification);
		}
	}
	return false;
}

bool UBaseContainer::RemoveItemBySlot(int32 Slot, EItemChangeReason ItemChangeReason, int32 Amount /*= -1*/, bool bShowNotification /*= false */)
{
	if (Items.IsValidIndex(Slot) && Items[Slot])
	{
		UBaseItem* anItem = Items[Slot];
		if (Amount == -1 || Amount == anItem->Amount)
		{
			Items[Slot] = NULL;

			if (GridComponent.Get())
			{
				GridComponent.Get()->NotifyClientItemStatus(ContainerType, ENotifyItemType::NOTIFY_ITEM_REMOVE, anItem, Slot, bShowNotification);
				GridComponent.Get()->OnRemoveItem.Broadcast(this, ContainerType, Slot);
			}

			return true;
		}
		else if (Amount < anItem->Amount)
		{
			return anItem->SetAmount(anItem->Amount - Amount, ItemChangeReason, bShowNotification);
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool UBaseContainer::TestRemoveItemBySlot(int32 Slot)
{
	if (!Items.IsValidIndex(Slot) || nullptr == Items[Slot])
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool UBaseContainer::UpdateItemBySlot(int32 Slot, FItemNetInfo itemInfo, EItemChangeReason ItemChangeReason, bool bShowNotification /*= false*/)
{
	if (Items.IsValidIndex(Slot) && Items[Slot])
	{
		UBaseItem* anItem = Items[Slot];
		if (nullptr == anItem)
		{
			return false;
		}
		anItem->InitFromNetInfo(itemInfo);

		//NoticeUpdateItem(anItem, ENotifyItemType::NOTIFY_ITEM_SETAMOUNT, bShowNotification);

		return true;
	}

	return false;
}

void UBaseContainer::ClearAllItem()
{
	Items.Empty();
	Items.SetNumZeroed(ItemMaxAmount);

	//NoticeUpdateItem(nullptr, ENotifyItemType::NOTIFY_INVENTORY_CLEARALL, false);
}

void UBaseContainer::NoticeUpdateItem(class UBaseItem* anItem, enum ENotifyItemType NotifyType, bool bShowNotification /*= false*/)
{
	if (GridComponent.Get())
	{
		int32 SlotIndex = -1;
		if (nullptr != anItem)
		{
			SlotIndex = anItem->SlotIndex;
		}
		GridComponent.Get()->NotifyClientItemStatus(ContainerType, NotifyType, anItem, -1, bShowNotification);
		GridComponent.Get()->OnUpdateItem.Broadcast(this, ContainerType, SlotIndex);
	}
}

bool UBaseContainer::IsValidIndex(int32 Slot)
{
	return Items.IsValidIndex(Slot);
}

class UBaseItem* UBaseContainer::FindItemBySlot(int32 Slot)
{
	if (Items.IsValidIndex(Slot) && Items[Slot])
	{
		return Items[Slot];
	}

	return nullptr;
}

bool UBaseContainer::ExchangeItem(int32 SourceSlot, class UBaseContainer* TargetContainer, int32 TargetSlot, bool bShowNotification /*= false*/)
{
	if (!CheckMove(SourceSlot, TargetContainer, TargetSlot))
	{
		return false;
	}

	class UBaseItem* SourceItem = FindItemBySlot(SourceSlot);
	class UBaseItem* TargetItem = TargetContainer->FindItemBySlot(TargetSlot);

	//测试移动SourceItem
	bool bTestSouce = false;
	if (nullptr != SourceItem)
	{
		bTestSouce = TargetContainer->TestAddItemToSlot(SourceItem, TargetSlot);
	}
	else
	{
		bTestSouce = TargetContainer->TestRemoveItemBySlot(TargetSlot);
	}

	if (!bTestSouce)
	{
		return false;
	}

	//测试移动TargetItem
	bool bTestTarget = false;
	if (nullptr != TargetItem)
	{
		bTestTarget = TestAddItemToSlot(TargetItem, SourceSlot);
	}
	else
	{
		bTestTarget = TestRemoveItemBySlot(SourceSlot);
	}

	if (!bTestTarget)
	{
		return false;
	}

	bool bResult = false;

	//移动SourceItem
	if (nullptr != SourceItem)
	{
		SourceItem->SlotIndex = TargetSlot;
		bResult = TargetContainer->AddItemToSlot(SourceItem, EItemChangeReason::Exchange, bShowNotification);
	}
	else
	{
		bResult = TargetContainer->RemoveItemBySlot(TargetSlot, EItemChangeReason::Exchange, -1, bShowNotification);
	}

	if (!bResult)
	{
		return false;
	}

	//移动TargetItem
	if (nullptr != TargetItem)
	{
		TargetItem->SlotIndex = SourceSlot;
		bResult = AddItemToSlot(TargetItem, EItemChangeReason::Exchange, bShowNotification);
	}
	else
	{
		bResult = RemoveItemBySlot(SourceSlot, EItemChangeReason::Exchange, -1, bShowNotification);
	}

	return bResult;
}

int32 UBaseContainer::MoveItem(int32 SourceSlot, int32 MoveAmount, class UBaseContainer* TargetContainer, int32 TargetSlot, bool bShowNotification /*= false*/)
{
	int32 HasMovedAmount = 0;
	class UBaseItem* SourceItem = FindItemBySlot(SourceSlot);
	if (nullptr == SourceItem)
	{
		return 0;
	}

	if (MoveAmount > SourceItem->Amount || MoveAmount <= 0)
	{
		return 0;
	}

	if (!CheckMove(SourceSlot, TargetContainer, TargetSlot))
	{
		return 0;
	}

	class UBaseItem* TargetItem = TargetContainer->FindItemBySlot(TargetSlot);
	if (nullptr != TargetItem)
	{
		if (TargetItem->CanMerge(SourceItem))
		{
			//可以合并
			int32 CanAddAmount = TargetItem->MaxAmount - TargetItem->Amount;
			int32 FinalMoveAmout = MoveAmount < CanAddAmount ? MoveAmount : CanAddAmount;
			SourceItem->SetAmount(SourceItem->Amount - FinalMoveAmout, EItemChangeReason::Use, bShowNotification);
			TargetItem->SetAmount(TargetItem->Amount + FinalMoveAmout, EItemChangeReason::Use, bShowNotification);

			return FinalMoveAmout;
		}
		else
		{
			//不能合并直接交换全部
			if (ExchangeItem(SourceSlot, TargetContainer, TargetSlot, bShowNotification))
			{
				return MoveAmount;
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		if (MoveAmount == SourceItem->Amount)
		{
			//全部移动
			if (ExchangeItem(SourceSlot, TargetContainer, TargetSlot, bShowNotification))
			{
				return MoveAmount;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if (GridComponent.Get())
			{
				FItemNetInfo newItemInfo;
				newItemInfo = SourceItem->GetItemNetInfo();
				newItemInfo.UniqueId = FGuid::NewGuid();
				newItemInfo.SlotIndex = TargetSlot;
				newItemInfo.Amount = MoveAmount;
				/*if (GridComponent.Get()->AddItemByNetInfo(InventoryType, newItemInfo, bShowNotification))
				{
					SourceItem->SetAmount(SourceItem->Amount - MoveAmount, EItemChangeReason::Use, bShowNotification);
					return MoveAmount;
				}
				else
				{
					return 0;
				}*/
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

bool UBaseContainer::CheckMove(int32 SourceSlot, class UBaseContainer* TargetContainer, int32 TargetSlot)
{
	if (nullptr == TargetContainer)
	{
		return false;
	}

	if (!IsValidIndex(SourceSlot))
	{
		return false;
	}

	if (!IsValidIndex(TargetSlot))
	{
		return false;
	}

	class UBaseItem* SourceItem = FindItemBySlot(SourceSlot);
	if (nullptr != SourceItem)
	{
		if (!SourceItem->CanMove())
		{
			return false;
		}
	}
	class UBaseItem* TargetItem = TargetContainer->FindItemBySlot(TargetSlot);
	if (nullptr != TargetItem)
	{
		if (!TargetItem->CanMove())
		{
			return false;
		}
	}

	return true;
}

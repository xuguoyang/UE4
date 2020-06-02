// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerControllerBase.h"
#include "RPGGameInstanceBase.h"
#include "RPGAssetManager.h"
#include "RPGSaveGame.h"
#include "RPGItem.h"

void ARPGPlayerControllerBase::BeginPlay()
{
    // 加载所有道具
    LoadInventory();

    Super::BeginPlay();
}

bool ARPGPlayerControllerBase::AddInventoryItem(URPGItem* NewItem, int32 ItemCount /*= 1*/, int32 ItemLevel /*= 1*/, bool bAutoSlot /*= true*/)
{
    bool bChanged = false;
    if (!NewItem)
    {
        UE_LOG(LogActionRPG, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
        return false;
    }

    if (ItemCount <= 0 || ItemLevel <= 0)
    {
        UE_LOG(LogActionRPG, Warning, TEXT("AddInventoryItem: Failed trying to add item %s with negative count or level!"), *NewItem->GetName());
        return false;
    }

    //获取当前Item数据
    FRPGItemData OldData;
    GetInventoryItemData(NewItem, OldData);

    // 更新下数据并与老数据进行比较并更新数据
    FRPGItemData NewData = OldData;
    NewData.UpdateItemData(FRPGItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);
    if (NewData != OldData)
    {
        InventoryData.Add(NewItem, NewData);
        NotifyInventoryItemChanged(true, NewItem);
        bChanged = true;
    }

    // 处理自动放进插槽
    if(bAutoSlot)
    {
        bChanged |= FillEmptySlotWithItem(NewItem);
    }

    // 数据有变动需要进行保存
    if (bChanged)
    {
        SaveInventory();
        return true;
    }

    return false;
}

bool ARPGPlayerControllerBase::GetInventoryItemData(URPGItem* Item, FRPGItemData& ItemData) const
{
    const FRPGItemData* FoundItem = InventoryData.Find(Item);

    if (FoundItem)
    {
        ItemData = *FoundItem;
        return true;
    }
    ItemData = FRPGItemData(0, 0);
    return false;
}

bool ARPGPlayerControllerBase::LoadInventory()
{
    InventoryData.Reset();
    SlottedItems.Reset();

    // 通过world获取GameInstance
    UWorld* World = GetWorld();
    URPGGameInstanceBase* GameInstance = World ? World->GetGameInstance<URPGGameInstanceBase>() : nullptr;

    if (!GameInstance)
    {
        return false;
    }

    // 绑定委托回调
    if (!GameInstance->OnSaveGameLoadedNative.IsBoundToObject(this))
    {
        GameInstance->OnSaveGameLoadedNative.AddUObject(this, &ARPGPlayerControllerBase::HandleSaveGameLoaded);
    }

    // ItemSlotsPerType中存放的插槽类型及其数量的列表，每个类型有多个插槽，构造所有的插槽放到SlottedItems中
    for (const TPair<FPrimaryAssetType, int32>& Pair: GameInstance->ItemSlotsPerType)
    {
        for (int32 SlotNumber = 0; SlotNumber < Pair.Value; SlotNumber++)
        {
            // 构造所有的插槽，当前都为空插槽
            SlottedItems.Add(FRPGItemSlot(Pair.Key, SlotNumber), nullptr);
        }
    }

    // 根据SaveGame.InventoryData数据填充InventoryData和SlottedItems
    URPGSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
    URPGAssetManager& AssetManager = URPGAssetManager::Get();
    if (CurrentSaveGame)
    {
        bool bFoundAnySlots = false;
        for (const TPair<FPrimaryAssetId, FRPGItemData>& ItemPair : CurrentSaveGame->InventoryData)
        {
            // 根据资源ID，创建一个道具配置对象
            URPGItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

            if (LoadedItem != nullptr)
            {
                InventoryData.Add(LoadedItem, ItemPair.Value);
            }
        }

        // 根据SaveGame.SlottedItems数据填充SlottedItems
        for (const TPair<FRPGItemSlot, FPrimaryAssetId>& ItemPair : CurrentSaveGame->SlottedItems)
        {
            // 根据资源ID，创建一个道具配置对象
            URPGItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Value);
            if (LoadedItem != nullptr)
            {
                SlottedItems.Add(ItemPair.Key, LoadedItem);
                bFoundAnySlots = true;
            }
        }

        if (!bFoundAnySlots)
        {
            FillEmptySlots();
        }

        // 发送装备加载完成通知
        NotifyInventoryLoaded();
        return true;
    }
    
    // 虽然加载失败，但是因为重置的装备所以还是要发送通知
    NotifyInventoryLoaded();
    return false;
}

bool ARPGPlayerControllerBase::SaveInventory()
{
    return true;
}

void ARPGPlayerControllerBase::FillEmptySlots()
{
    bool bShouldSave = false;
    for (const TPair<URPGItem*, FRPGItemData>& Pair : InventoryData)
    {
        bShouldSave |= FillEmptySlotWithItem(Pair.Key);
    }

    if (bShouldSave)
    {
        SaveInventory();
    }
}

void ARPGPlayerControllerBase::HandleSaveGameLoaded(URPGSaveGame* NewSaveGame)
{
    LoadInventory();
}

bool ARPGPlayerControllerBase::FillEmptySlotWithItem(URPGItem* NewItem)
{
    // 找一个对应资源类型型的空的槽插入这个Item
    FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;

    // 找到空槽的KEY
    FRPGItemSlot EmptySlot;
    for (TPair<FRPGItemSlot, URPGItem*>& Pair : SlottedItems)
    {
        // 类型相同
        if (Pair.Key.ItemType == NewItemType)
        {
            // Item已经存在，不处理
            if (Pair.Value == NewItem)
            {
                return false;
            }
            else if (Pair.Value == nullptr && (!EmptySlot.IsValid()) || EmptySlot.SlotNumber > Pair.Key.SlotNumber)
            {
                EmptySlot = Pair.Key;
            }
        }
    }

    if (EmptySlot.IsValid())
    {
        SlottedItems[EmptySlot] = NewItem;
        NotifySlottedItemChanged(EmptySlot, NewItem);
        return true;
    }

    return false;
}

void ARPGPlayerControllerBase::NotifyInventoryItemChanged(bool bAdded, URPGItem* Item)
{
    
}

void ARPGPlayerControllerBase::NotifySlottedItemChanged(FRPGItemSlot ItemSlot, URPGItem* Item)
{
    OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
    OnSlottedItemChanged.Broadcast(ItemSlot, Item);

    // 执行蓝图回调函数
    SlottedItemChanged(ItemSlot, Item);
}

void ARPGPlayerControllerBase::NotifyInventoryLoaded()
{

}

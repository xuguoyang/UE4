// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "RPGInventoryInterface.h"
#include "RPGPlayerControllerBase.generated.h"


/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGPlayerControllerBase : public APlayerController, public IRPGInventoryInterface
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

    // 添加一个新的装备道具
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool AddInventoryItem(URPGItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

    /**删除一个装备道具 */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool RemoveInventoryItem(URPGItem* RemovedItem, int32 RemoveCount = 1);

    /**根据道具类型，获取道具列表 */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void GetInventoryItems(TArray<URPGItem*>& Items, FPrimaryAssetType ItemType);

    /**返回 Inventory中Item的数量 */
    UFUNCTION(BlueprintPure, Category = Inventory)
    int32 GetInventoryItemCount(URPGItem* Item) const;

    // 获取装备数据
    UFUNCTION(BlueprintPure, Category = Inventory)
    bool GetInventoryItemData(URPGItem* Item, FRPGItemData& ItemData) const;

    /** 设置slot中的值，传入空指针清空对应slot */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool SetSlottedItem(FRPGItemSlot ItemSlot, URPGItem* Item);

    /** 获取slot中对应的Item */
    UFUNCTION(BlueprintPure, Category = Inventory)
    URPGItem* GetSlottedItem(FRPGItemSlot ItemSlot) const;

    /** 返回slots中指定type的所有Item */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void GetSlottedItems(TArray<URPGItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

    /** 玩家拥有的Item列表(数量和等级) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
    TMap<URPGItem*, FRPGItemData> InventoryData;

    /** 玩家装备栏中的道具*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
    TMap<FRPGItemSlot, URPGItem*> SlottedItems;

    /** 从保存数据加载装备到数组中 */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool LoadInventory();

    /** 手动保存装备数据*/
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool SaveInventory();

    /**使用装备中的道具填充空的插槽*/
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void FillEmptySlots();

    /** Native version above, called before BP delegate */
    FOnSlottedItemChangedNative OnSlottedItemChangedNative;

    /** Delegate called when an inventory slot has changed */
    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnSlottedItemChanged OnSlottedItemChanged;

    /** Delegate called when the inventory has been loaded/reloaded */
    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnInventoryLoaded OnInventoryLoaded;

    /** Native version above, called before BP delegate */
    FOnInventoryLoadedNative OnInventoryLoadedNative;

    FOnInventoryItemChangedNative OnInventoryItemChangedNative;
    
    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnInventoryItemChanged OnInventoryItemChanged;

	/** Called after the inventory was changed and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, URPGItem* Item);

    // 蓝图回调函数
    UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
    void SlottedItemChanged(FRPGItemSlot ItemSlot, URPGItem* Item);

    /** 实现IRPGInventoryInterface接口的函数*/
    virtual const TMap<URPGItem*, FRPGItemData>& GetInventoryDataMap() const override
    {
        return InventoryData;
    }
    virtual const TMap<FRPGItemSlot, URPGItem*>& GetSlottedItemMap() const override
    {
        return SlottedItems;
    }
    virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() override
    {
        return OnInventoryItemChangedNative;
    }
    virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
    {
        return OnSlottedItemChangedNative;
    }
    virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
    {
        return OnInventoryLoadedNative;
    }
protected:
    /** Called when a global save game as been loaded */
    void HandleSaveGameLoaded(URPGSaveGame* NewSaveGame);
   
    /** 自动找一个空的槽位插入Item */
    bool FillEmptySlotWithItem(URPGItem* NewItem);

    /** 执行装备相关更新后的委托回调*/
    void NotifyInventoryItemChanged(bool bAdded, URPGItem* Item);
    void NotifySlottedItemChanged(FRPGItemSlot ItemSlot, URPGItem* Item);
    void NotifyInventoryLoaded();
};

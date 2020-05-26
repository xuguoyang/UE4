#pragma once
#include "UObject/PrimaryAssetId.h"
#include "RPGType.generated.h"
/**
*
*/
class URPGItem;

/** 道具装备插槽定义*/
USTRUCT(BlueprintType)
struct MYACTIONRPG_API FRPGItemSlot
{
    GENERATED_BODY()
        /** Constructor, -1 means an invalid slot */
    FRPGItemSlot()
    : SlotNumber(-1)
    {}

    FRPGItemSlot(const FPrimaryAssetType& InItemType, int32 InSlotNumber)
        : ItemType(InItemType)
        , SlotNumber(InSlotNumber)
    {}

    /** 可以放进当前格子的道具类型*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    FPrimaryAssetType ItemType;

    /** 格子序号*/ 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    int32 SlotNumber;

    /** Equality operators */
    bool operator==(const FRPGItemSlot& Other) const
    {
        return ItemType == Other.ItemType && SlotNumber == Other.SlotNumber;
    }

    bool operator!=(const FRPGItemSlot& Other) const
    {
        return !(*this == Other);
    }

    /** 实现用于TMap Key值的GetTypeHash函数 */
    friend inline uint32 GetTypeHash(const FRPGItemSlot& Key)
    {
        uint32 Hash = 0;

        Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
        Hash = HashCombine(Hash, (uint32)Key.SlotNumber);
        return Hash;
    }

    /** Returns true if slot is valid */
    bool IsValid() const
    {
        return ItemType.IsValid() && SlotNumber >= 0;
    }
};

USTRUCT(BlueprintType)
struct MYACTIONRPG_API FRPGItemData
{
    GENERATED_BODY()

        FRPGItemData() :
        ItemCount(1),
        ItemLevel(1) {}

    FRPGItemData(int32 InItemCount, int32 InItemLevel)
        : ItemCount(InItemCount)
        , ItemLevel(InItemLevel)
    {}

    /** 道具实例的数量，永远不能小于1 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    int32 ItemCount;

    /** 道具等级，永远不能小于1*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    int32 ItemLevel;

    bool operator==(const FRPGItemData& Other) const
    {
        return ItemCount == Other.ItemCount && ItemLevel == Other.ItemLevel;
    }

    bool operator!=(const FRPGItemData& Other) const
    {
        return !(*this == Other);
    }

    bool IsValid() const
    {
        return ItemCount > 0;
    }

    /** 更新ItemData*/
    void UpdateItemData(const FRPGItemData& Other, int32 MaxCount, int32 MaxLevel)
    {
        if (MaxCount <= 0)
        {
            MaxCount = MAX_int32;
        }

        if (MaxLevel <= 0)
        {
            MaxLevel = MAX_int32;
        }

        ItemCount = FMath::Clamp(ItemCount + Other.ItemCount, 1, MaxCount);
        ItemLevel = FMath::Clamp(Other.ItemLevel, 1, MaxLevel);
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, bool, bAdded, URPGItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedNative, bool, URPGItem*);

/** Delegate called when the contents of an inventory slot change */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, FRPGItemSlot, ItemSlot, URPGItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedNative, FRPGItemSlot, URPGItem*);

/** Delegate called when the entire inventory has been loaded, all items may have been replaced */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoaded);
DECLARE_MULTICAST_DELEGATE(FOnInventoryLoadedNative);

/**声明加载完保存数据的回调委托 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoaded, URPGSaveGame*, SaveGame);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoadedNative, URPGSaveGame*);
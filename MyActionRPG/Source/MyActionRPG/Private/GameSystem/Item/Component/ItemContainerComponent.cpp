// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Item/Component/ItemContainerComponent.h"
#include "GameAsset/TableDataAsset/TableAssetCommon.h"
#include "GameAsset/TableDefine/RPGItemDataTableRow.h"
#include "GameSystem/Item/ItemBase.h"
#include <Engine/ActorChannel.h>

// Sets default values for this component's properties
UItemContainerComponent::UItemContainerComponent():
 Capacity(10)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}


void UItemContainerComponent::InitCapacity(int32 nCapacity)
{
    if (Capacity <= 0)
    {
        return;
    }

    ItemArray.Reserve(Capacity);
    ItemArray.SetNum(Capacity);
    for (int i = 0; i < Capacity; ++i)
    {
        ItemArray[i].Index = i;
    }
}

void UItemContainerComponent::ServerRemoveItemByObj_Implementation(UItemBase* Item)
{
    RemoveItemByObj(Item);
}

void UItemContainerComponent::ServerRemoveItemByGird_Implementation(int32 GirdIndex)
{
    RemoveItemByGird(GirdIndex);
}

void UItemContainerComponent::ServerRemoveItemByItemID_Implementation(const FName& ItemID)
{
    RemoveItemByItemID(ItemID);
}

UItemBase* UItemContainerComponent::AddItemByItemID(const FName& ItemID)
{
    if (GetOwnerRole() != ENetRole::ROLE_Authority)
    {
        return nullptr;
    }

    UItemBase* Item = CreateNewItem(ItemID);
    if (AddItemByObj(Item) >= 0)
    {
        return Item;
    }

    return nullptr;
}

int32 UItemContainerComponent::AddItemByObj(UItemBase* Item)
{
    if (Item == nullptr)
    {
        return -1;
    }

    OutGirdArray EmptyGirds;
    if (!GetEmptyGird(EmptyGirds))
    {
        return -1;
    }

    int32 Index = EmptyGirds[0];
    ItemArray[Index].Item = Item;
    Item->OnAdd(this);

    return Index;
}

bool UItemContainerComponent::RemoveItemByGird(int32 Index)
{
    if (!IsIndexVaild(Index))
    {
        return false;
    }

    if (ItemArray[Index].Item == nullptr)
    {
        return false;
    }

    ItemArray[Index].Reset();
    return true;
}

bool UItemContainerComponent::RemoveItemByItemID(const FName& ItemID)
{
    if (ItemID.IsNone())
    {
        return false;
    }

    bool bRemove = false;
    for (FItemGird& Gird: ItemArray)
    {
        if (Gird.Item != nullptr && Gird.Item->ItemID == ItemID)
        {
            Gird.Reset();
            bRemove = true;
        }
    }
    
    return bRemove;
}

bool UItemContainerComponent::RemoveItemByObj(UItemBase* Item)
{
    for (FItemGird& Gird : ItemArray)
    {
        if (Gird.Item != nullptr && Gird.Item == Item)
        {
            Gird.Reset();
            return true;
        }
    }

    return false;
}

UItemBase* UItemContainerComponent::GetItemByIndex(int32 Index)
{
    if (!IsIndexVaild(Index))
    {
        return nullptr;
    }

    return ItemArray[Index].Item;
}

bool UItemContainerComponent::GetEmptyGird(OutGirdArray& OutGird, uint32 Num /*= 1*/)
{
    OutGird.Reserve(Num);
    for (int i = 0; i < ItemArray.Num(); ++i)
    {
        if (ItemArray[i].IsEmpty())
        {
            OutGird.Add(i);
        }

        if (OutGird.Num() == Num)
        {
            return true;
        }
    }

    return false;
}

bool UItemContainerComponent::HasItem(UItemBase* Item)
{
    if (Item == nullptr)
    {
        return false;
    }

    for (FItemGird& Gird:ItemArray)
    {
        if (!Gird.IsEmpty() && Gird.Item == Item)
        {
            return true;
        }
    }

    return false;
}

// Called when the game starts
void UItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();


    if (GetWorld()->GetNetMode() != ENetMode::NM_Client)
    {
        InitCapacity(Capacity);
    }
}


bool UItemContainerComponent::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
    bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
    for (FItemGird& ItemGird: ItemArray)
    {
        if (ItemGird.Item && !ItemGird.Item->IsPendingKill())
        {
            WroteSomething |= Channel->ReplicateSubobject(ItemGird.Item, *Bunch, *RepFlags);
        }
    }

    return WroteSomething;
}

class UItemBase* UItemContainerComponent::CreateNewItem(const FName& ItemID)
{
    FRPGItemDataTableRow* ItemConfig = UTableAssetCommon::Get()->TableItem->FindRow<FRPGItemDataTableRow>(ItemID, TEXT(""));
    if (nullptr == ItemConfig)
    {
        return nullptr;
    }

    UItemBase* Item = NewObject<UItemBase>(this, ItemConfig->ItemObjClass);
    Item->ItemID = ItemID;
    Item->ItemConfig = ItemConfig;

    return Item;
}

void UItemContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 背包只对自己可视
    DOREPLIFETIME_CONDITION(UItemContainerComponent, ItemArray, COND_OwnerOnly);
}

void UItemContainerComponent::OnRep_ItemArrayChange()
{
    ItemArray[0].Item;
}

bool UItemContainerComponent::IsIndexVaild(int32 Index)
{
    return Index >= 0 && Index < ItemArray.Num();
}


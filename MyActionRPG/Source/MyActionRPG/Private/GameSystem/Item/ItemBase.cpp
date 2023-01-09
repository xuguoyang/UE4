// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Item/ItemBase.h"
#include "GameSystem/Item/Component/ItemContainerComponent.h"

UItemBase::UItemBase():
ItemLevel(0),
ItemCount(1),
ItemConfig(nullptr),
ItemActor(nullptr),
OwnerActor(nullptr),
OwnerContainerComponent(nullptr)
{
    
}

bool UItemBase::IsSupportedForNetworking() const
{
    return true;
}

void UItemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UItemBase, ItemID);
    DOREPLIFETIME(UItemBase, ItemLevel);
}

void UItemBase::OnAdd(UItemContainerComponent* ContainerComponent)
{
    if (!ContainerComponent)
    {
        return;
    }

    OwnerContainerComponent = ContainerComponent;
    OwnerActor = ContainerComponent->GetOwner();
}

void UItemBase::OnRemove()
{
    OwnerContainerComponent = nullptr;
    OwnerActor = nullptr;
}

void UItemBase::OnRep_ItemID()
{
    // 客户端获取FRPGItemDataTableRow的指针并缓存着，方便查询
    if (UTableAssetCommon::Get() == nullptr)
    {
        return;
    }

    ItemConfig = UTableAssetCommon::Get()->TableItem->FindRow<FRPGItemDataTableRow>(ItemID, TEXT(""));
}

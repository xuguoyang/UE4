// Fill out your copyright notice in the Description page of Project Settings.


#include "ShortcutComponent.h"
#include "ShortcutUseInterface.h"

bool FShortcutGird::UseItem()
{
    IShortcutUseInterface*  IShortcutUse = Cast<IShortcutUseInterface>(Item);
    if (IShortcutUse)
    {
        return IShortcutUse->ShortcutUse();
    }

    return false;
}

void FShortcutGird::Add(UObject* InItem)
{
    Item = InItem;
    IShortcutUseInterface*  IShortcutUse = Cast<IShortcutUseInterface>(Item);
    if (IShortcutUse)
    {
        IShortcutUse->OnAddShortcut();
    }
}

void FShortcutGird::Clear()
{
    Item = nullptr;
    IShortcutUseInterface*  IShortcutUse = Cast<IShortcutUseInterface>(Item);
    if (IShortcutUse)
    {
        IShortcutUse->OnRemoveShortcut();
    }
}

bool FShortcutGird::CanAdd(UObject* InItem)
{
    IShortcutUseInterface*  IShortcutUse = Cast<IShortcutUseInterface>(InItem);
    if (!IShortcutUse || IShortcutUse->GetType() != Type)
    {
        return false;
    }

    return true;
}

// Sets default values for this component's properties
UShortcutComponent::UShortcutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts
void UShortcutComponent::BeginPlay()
{
	Super::BeginPlay();

    // 初始化快捷栏的索引
	for (int32 Index = 0; Index < ShortcutGirds.Num(); ++Index)
	{
        ShortcutGirds[Index].Index = Index;
	}
	
}

void UShortcutComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UShortcutComponent::AddShortcutItem(UObject* Item, int32 Index)
{
    // 找一个空的格子
    if (Index == -1)
    {
        Index = GetEmptyGird(GetShortcutType(Item));
    }

    if (!CanAdd(Index, Item))
    {
        return false;
    }

    // 先移除之前的
    RemoveShortcutItem(Index);

    // 添加新的
    ShortcutGirds[Index].Add(Item);

    // 通知客户端添加成功
    ClientAddSuccessed(Index);

    return true;
}

bool UShortcutComponent::RemoveShortcutItem(int32 Index)
{
    if (!CanRemove(Index))
    {
        return false;
    }

    ShortcutGirds[Index].Clear();

    // 通知客户端移除成功
    ClientRemoveSuccessed(Index);

    return true;
}

bool UShortcutComponent::ShortcutUse(int32 Index)
{
    if (!CanUse(Index))
    {
        return false;
    }

    return ShortcutGirds[Index].UseItem();
}



EShortcutType UShortcutComponent::GetShortcutType(UObject* Item)
{
    IShortcutUseInterface*  IShortcutUse = Cast<IShortcutUseInterface>(Item);
    if (IShortcutUse)
    {
        return IShortcutUse->GetType();
    }

    return EShortcutType::ShortcutGirdType_None;
}

void UShortcutComponent::ClientRemoveSuccessed_Implementation(int32 Index)
{

}

void UShortcutComponent::ClientAddSuccessed_Implementation(int32 Index)
{

}

bool UShortcutComponent::IsIndexVaild(int32 Index)
{
    return Index >= 0 && Index < ShortcutGirds.Num();
}

bool UShortcutComponent::CanAdd(int32 Index, UObject* Item)
{
    if (Item == nullptr)
    {
        return false;
    }

    if (!IsIndexVaild(Index))
    {
        return false;
    }

    if (!ShortcutGirds[Index].CanAdd(Item))
    {
        return false;
    }

    return true;
}

bool UShortcutComponent::CanRemove(int32 Index)
{
    if (!IsIndexVaild(Index))
    {
        return false;
    }

    if (ShortcutGirds[Index].Item == nullptr)
    {
        return false;
    }

    return true;
}

bool UShortcutComponent::CanUse(int32 Index)
{
    if (!IsIndexVaild(Index))
    {
        return false;
    }

    if (ShortcutGirds[Index].Item == nullptr)
    {
        return false;
    }

    return true;
}

int32 UShortcutComponent::GetEmptyGird(EShortcutType Type)
{
    for (int32 Index = 0; Index < ShortcutGirds.Num(); ++Index)
    {
        if (ShortcutGirds[Index].Type == Type && ShortcutGirds[Index].Item == nullptr)
        {
            return Index;
        }
    }

    return -1;
}

void UShortcutComponent::ServerRemoveShortcutItem_Implementation(int32 Index)
{
    RemoveShortcutItem(Index);
}

bool UShortcutComponent::ServerRemoveShortcutItem_Validate(int32 Index)
{
    return true;
}

void UShortcutComponent::ServerAddShortcutItem_Implementation(int32 Index, UObject* Item)
{
    AddShortcutItem(Item, Index);
}

bool UShortcutComponent::ServerAddShortcutItem_Validate(int32 Index, UObject* Item)
{
    return true;
}

void UShortcutComponent::ServerShortcutUse_Implementation(int32 Index)
{
    ShortcutUse(Index);
}

bool UShortcutComponent::ServerShortcutUse_Validate(int32 Index)
{
    return true;
}



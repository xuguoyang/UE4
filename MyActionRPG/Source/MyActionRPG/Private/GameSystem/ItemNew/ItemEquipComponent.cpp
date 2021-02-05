// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEquipComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UItemEquipComponent::UItemEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


bool UItemEquipComponent::EquipItem(UItemBase* Item)
{
    // 获取对应类型的装备栏看看是否符合条件
    if (!CanEquipItem(Item))
    {
        return false;
    }

    // 获取对应类型的装备栏
    OutGirdArray OutArrary;
    if (!GetEquipGird(Item->ItemConfig->ItemEquipType, OutArrary))
    {
        return false;
    }

    // 当前有空的位置，直接添加
    bool bAdd = false;
    for (auto& Index : OutArrary)
    {
        if (EquipArray[Index].IsEmpty())
        {
            EquipArray[Index].Item = Item;
            bAdd = true;
            break;
        }
    }

    // 没有的话替换第一个位置的道具
    if (bAdd == false)
    {
        DestoryItemActor(EquipArray[OutArrary[0]].Item);
        EquipArray[OutArrary[0]].Item = Item;
        bAdd = true;
    }

    // 成功添加，刷新显示
    if (bAdd)
    {
        LoadItemActor(Item);
    }

    return bAdd;
}

bool UItemEquipComponent::RemoveItem(int32 Index)
{
    if (Index < 0 || Index >= EquipArray.Num())
    {
        return false;
    }

    if (EquipArray[Index].IsEmpty())
    {
        return false;
    }

    EquipArray[Index].Reset();

    return true;
}

bool UItemEquipComponent::CanEquipItem(UItemBase* Item)
{
    if (Item == nullptr)
    {
        return false;
    }

    if (Item->ItemConfig == nullptr)
    {
        return false;
    }

    // 类型是否合法 
    if (Item->ItemConfig->ItemEquipType == EItemEquipType::ItemEquipType_None)
    {
        return false;
    }

    // 道具本身的可装备条件是否满足（角色等级、力量、敏捷、智力等属性需要达到一定要求）

    return true;
}

// Called when the game starts
void UItemEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UItemEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UItemEquipComponent, EquipArray);
}


void UItemEquipComponent::OnRep_EquipArrayChange()
{

}

bool UItemEquipComponent::LoadItemActor(UItemBase* Item)
{
    // 加载actor并绑定角色身上
    USkeletalMeshComponent* pMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
    if (pMesh == nullptr)
    {
        return false;
    }

    // 创建道具对应的Actor
    AActor* pItemActor = GetWorld()->SpawnActor(Item->ItemConfig->ItemActorClass);
    if (pItemActor != nullptr)
    {
        pItemActor->AttachToComponent(pMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Item->ItemConfig->BindSocketName);
        Item->ItemActor = pItemActor;
        return true;
    }

    return false;
}

bool UItemEquipComponent::DestoryItemActor(UItemBase* Item)
{
    if (Item->ItemActor == nullptr || Item->ItemActor->IsPendingKill())
    {
        return false;
    }

    Item->ItemActor->Destroy();

    return true;
}

bool UItemEquipComponent::GetEquipGird(EItemEquipType Type, OutGirdArray& OutArrary)
{
    OutArrary.Empty();
    if (Type == EItemEquipType::ItemEquipType_None)
    {
        return false;
    }

    for (int32 Index = 0; Index < EquipArray.Num(); ++Index)
    {
        if (EquipArray[Index].Type == Type)
        {
            OutArrary.Add(Index);
        }
    }

    if(OutArrary.Num() > 0)
    {
        return true;
    }

    return false;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Item/Component/ItemEquipComponent.h"
#include <Components/SkeletalMeshComponent.h>

// Sets default values for this component's properties
UItemEquipComponent::UItemEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


void UItemEquipComponent::ServerEquipItem_Implementation(UItemBase* Item)
{
    if (Item == nullptr)
    {
        return;
    }

    EquipItem(Item);
}

bool UItemEquipComponent::EquipItem(UItemBase* Item)
{
    // ��ȡ��Ӧ���͵�װ���������Ƿ��������
    if (!CanEquipItem(Item))
    {
        return false;
    }

    // ��ȡ��Ӧ���͵�װ����
    OutGirdArray OutArrary;
    if (!GetEquipGird(Item->ItemConfig->ItemEquipType, OutArrary))
    {
        return false;
    }

    // ��ǰ�пյ�λ�ã�ֱ�����
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

    // û�еĻ��滻��һ��λ�õĵ���
    if (bAdd == false)
    {
        DestoryItemActor(EquipArray[OutArrary[0]].Item);
        EquipArray[OutArrary[0]].Item = Item;
        bAdd = true;
    }

    // �ɹ���ӣ�ˢ����ʾ
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

    // �����Ƿ�Ϸ� 
    if (Item->ItemConfig->ItemEquipType == EItemEquipType::ItemEquipType_None)
    {
        return false;
    }

    // ���߱���Ŀ�װ�������Ƿ����㣨��ɫ�ȼ������������ݡ�������������Ҫ�ﵽһ��Ҫ��

    return true;
}

UItemBase* UItemEquipComponent::FindItemByType(EItemEquipType Type)
{
    if (Type == EItemEquipType::ItemEquipType_None)
    {
        return nullptr;
    }

    for (FEquipGird& Gird: EquipArray)
    {
        if (Gird.Type == Type)
        {
            return Gird.Item;
        }
    }

    return nullptr;
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
    if (Item->ItemConfig->ItemActorClass == nullptr)
    {
        return false;
    }

    // ����actor���󶨽�ɫ����
    USkeletalMeshComponent* pMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
    if (pMesh == nullptr)
    {
        return false;
    }

    // �������߶�Ӧ��Actor
    FActorSpawnParameters Parameters;
    Parameters.Owner = GetOwner();
    Parameters.Instigator = Cast<APawn>(GetOwner());
    AActor* pItemActor = GetWorld()->SpawnActor<AActor>(Item->ItemConfig->ItemActorClass, Parameters);
    if (pItemActor != nullptr)
    {
        pItemActor->SetReplicates(true);
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


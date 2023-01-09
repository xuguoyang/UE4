// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/RPGItemTypes.h"
#include "ItemEquipComponent.generated.h"

class UItemBase;

// װ������
USTRUCT(BlueprintType)
struct FEquipGird
{
    GENERATED_USTRUCT_BODY()

    FEquipGird() :Item(nullptr) {}

    UPROPERTY(EditAnywhere)
    EItemEquipType Type;                   // ��ǰλ��װ������

    UPROPERTY()
    class UItemBase* Item;                 // ��ǰ����

    bool IsEmpty()
    {
        return Item == nullptr;
    }

    void Reset()
    {
        Item = nullptr;
    }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UItemEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemEquipComponent();
// �ͻ����������������Ϣ
public:
    /** �����������װ��������Ϣ*/
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerEquipItem(UItemBase* Item);
public:
    /** װ������ */
    UFUNCTION(BlueprintCallable, Category = Equip)
    bool EquipItem(UItemBase* Item);

    /** ж�ص���*/
    UFUNCTION(BlueprintCallable, Category = Equip)
    bool RemoveItem(int32 Index);

    UFUNCTION(BlueprintCallable, Category = Equip)
    bool CanEquipItem(UItemBase* Item);

    /** ��ȡĳ�����͵�װ��Itemָ��*/
    UItemBase* FindItemByType(EItemEquipType Type);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
    /** װ�����ӵ���������ͼ������*/
    UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_EquipArrayChange, Category = Equip)
    TArray<FEquipGird> EquipArray;

private:
    UFUNCTION()
    void OnRep_EquipArrayChange();

private:
    bool LoadItemActor(UItemBase* Item);

    bool DestoryItemActor(UItemBase* Item);
private:
    /**��ȡĳ���������е�װ�������*/
    typedef TArray<uint8> OutGirdArray;
    bool GetEquipGird(EItemEquipType Type, OutGirdArray& OutArrary);
};

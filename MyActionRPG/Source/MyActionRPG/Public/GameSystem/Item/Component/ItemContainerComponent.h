// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/RPGItemTypes.h"
#include "ItemContainerComponent.generated.h"

class UItemBase;

// ��������
USTRUCT(BlueprintType)
struct FItemGird
{
    GENERATED_USTRUCT_BODY()

    FItemGird() :Item(nullptr) {}

    UPROPERTY()
    uint32 Index;                   // ����

    UPROPERTY(BlueprintReadOnly)
    class UItemBase* Item;          // ��ǰ����

    bool IsEmpty()
    {
        return Item == nullptr;
    }

    void Reset()
    {
        Item = nullptr;
    }
};

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemContainerComponent();

    UFUNCTION(BlueprintCallable, Category = ITEM)
    void InitCapacity(int32 nCapacity);

    // �ͻ������������������
public:
    /**�ͻ��˷��������Ƴ�ĳ�����ߵ�RPC*/
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerRemoveItemByObj(UItemBase* Item);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerRemoveItemByGird(int32 GirdIndex);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerRemoveItemByItemID(const FName& ItemID);

public:
    /** ��ӵ��ߣ�ֱ�Ӵ�����ӵ�������*/ 
    UFUNCTION(BlueprintCallable, Category = ITEM)
    UItemBase* AddItemByItemID(const FName& ItemID);

    /** ��ӵ��ߣ�ͨ������ָ��*/ 
    UFUNCTION(BlueprintCallable, Category = ITEM)
    int32 AddItemByObj(UItemBase* Item);

    /** ɾ�����ߣ�ͨ������*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool RemoveItemByGird(int32 Index);

    /** ɾ�����ߣ�ͨ��ID*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool RemoveItemByItemID(const FName& ItemID);

    /** ɾ�����ߣ�ͨ��ID*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool RemoveItemByObj(UItemBase* Item);

    /** ��ȡ���ߣ�ͨ������*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = ITEM)
    UItemBase* GetItemByIndex(int32 Index);

    /** ��ȡn���յĸ���*/
    typedef TArray<uint32> OutGirdArray;
    bool GetEmptyGird(OutGirdArray& OutGird, uint32 Num = 1);

    /** ��ǰ�Ƿ�ӵ��ĳ������*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool HasItem(UItemBase* Item);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    /** �������߶���*/
    class UItemBase* CreateNewItem(const FName& ItemID);
    
	
protected:
    // ��ǰ��������
    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ItemArrayChange, Category = ITEM)
    TArray<FItemGird> ItemArray;

    /** ������С*/ 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default)
    int32 Capacity;
protected:
    UFUNCTION()
    void OnRep_ItemArrayChange();

private:
    /** ��ǰ�����Ƿ�Ϸ�*/
    bool IsIndexVaild(int32 Index);
};

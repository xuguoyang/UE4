// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Array.h"
#include "Define/RPGItemTypes.h"
#include "ItemContainerComponent.generated.h"

class UItemBase;

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemContainerComponent();

    UFUNCTION(BlueprintCallable, Category = ITEM)
    void InitCapacity(int32 nCapacity);

    // 客户端向服务器发送请求
public:
    /**客户端发送请求移除某个道具的RPC*/
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ClientRequestRemoveItemByObj(UItemBase* Item);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ClientRequestRemoveItemByGird(int32 GirdIndex);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ClientRequestRemoveItemByItemID(const FName& ItemID);

public:
    /** 添加道具，直接创建添加到容器中*/ 
    UFUNCTION(BlueprintCallable, Category = ITEM)
    UItemBase* AddItemByItemID(const FName& ItemID);

    /** 添加道具，通过道具指针*/ 
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool AddItemByObj(UItemBase* Item);

    /** 删除道具，通过索引*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool RemoveItemByGird(int32 Index);

    /** 删除道具，通过ID*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool RemoveItemByItemID(const FName& ItemID);

    /** 删除道具，通过ID*/
    UFUNCTION(BlueprintCallable, Category = ITEM)
    bool RemoveItemByObj(UItemBase* Item);

    /** 获取道具，通过索引*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = ITEM)
    UItemBase* GetItemByIndex(int32 Index);

    /** 获取n个空的格子*/
    typedef TArray<uint32> OutGirdArray;
    bool GetEmptyGird(OutGirdArray& OutGird, uint32 Num = 1);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    /** 创建道具对象*/
    class UItemBase* CreateNewItem(const FName& ItemID);
    
	
protected:
    // 当前格子容器
    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ItemArrayChange, Category = ITEM)
    TArray<FItemGird> ItemArray;

    /** 容器大小*/ 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
    int32 Capacity;
protected:
    UFUNCTION()
    void OnRep_ItemArrayChange();

private:
    /** 当前索引是否合法*/
    bool IsIndexVaild(int32 Index);
};

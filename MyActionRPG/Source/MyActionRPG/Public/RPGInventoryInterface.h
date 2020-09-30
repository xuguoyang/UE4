// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGInventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPGInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYACTIONRPG_API IRPGInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /** 获取拥有的Item所有的Data*/
    virtual const TMap<URPGItem*, FRPGItemData>& GetInventoryDataMap() const = 0;

    /** 获取当前装备栏中的道具*/
    virtual const TMap<FRPGItemSlot, URPGItem*>& GetSlottedItemMap() const = 0;

    /** 获取道具变化的回调委托*/
    virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() = 0;

    /** 获取装备栏发生变化时的回调委托*/
    virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() = 0;

    /** 获取装备加载时的回调委托*/
    virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() = 0;
};

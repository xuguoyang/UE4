// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGType.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstanceBase.generated.h"

/**
 * 生成基于此类的蓝图，在蓝图中添加相关的逻辑
 * 主要负责游戏数据的保存，和游戏对应状态的事件处理
 */
UCLASS()
class MYACTIONRPG_API URPGGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
    URPGGameInstanceBase();

    /**新玩家默认的装备道具列表*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
    TMap<FPrimaryAssetId, FRPGItemData> DefaultInventory;

    /**每个类型的插槽数量*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
    TMap<FPrimaryAssetType, int32> ItemSlotsPerType;

    /**加载保存数据的原生委托*/
    FOnSaveGameLoadedNative OnSaveGameLoadedNative;

    /**加载保存数据完成后调用的委托*/
    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnSaveGameLoaded OnSaveGameLoaded;

    /**返回保存数据的对象指针*/
    UFUNCTION(BlueprintCallable, Category = Save)
    URPGSaveGame* GetCurrentSaveGame();

    /**给玩家添加默认道具*/
    void AddDefaultInventory(URPGSaveGame* SaveGame, bool bRemoveExtra);
protected:
    UPROPERTY()
    URPGSaveGame* CurrentSaveGame;
};

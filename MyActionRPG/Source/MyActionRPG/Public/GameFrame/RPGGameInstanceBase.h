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

    /**给玩家添加默认道具*/
    UFUNCTION(BlueprintCallable, Category = Inventory)
    void AddDefaultInventory(URPGSaveGame* SaveGame, bool bRemoveExtra);

    /**加载保存数据的原生委托*/
    FOnSaveGameLoadedNative OnSaveGameLoadedNative;

    /**加载保存数据完成后调用的委托*/
    UPROPERTY(BlueprintAssignable, Category = Inventory)
    FOnSaveGameLoaded OnSaveGameLoaded;

    /**返回保存数据的对象指针*/
    UFUNCTION(BlueprintCallable, Category = Save)
    URPGSaveGame* GetCurrentSaveGame();

   /**是否开启存档功能*/
    UFUNCTION(BlueprintCallable, Category = Save)
    void SetSavingEnabled(bool bEnabled);

    /**加载完存档数据后的回调 */
    UFUNCTION(BlueprintCallable, Category = Save)
    bool HandleSaveGameLoaded(USaveGame* SaveGameObject);

    /**获取保存数据的slot和index */
    UFUNCTION(BlueprintCallable, Category = Save)
    void GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const;

    /**执行数据保存 */
    UFUNCTION(BlueprintCallable, Category = Save)
    bool WriteSaveGame();

    /** The slot name used for saving */
    UPROPERTY(BlueprintReadWrite, Category = Save)
    FString SaveSlot;

    /** The platform-specific user index */
    UPROPERTY(BlueprintReadWrite, Category = Save)
    int32 SaveUserIndex;
protected:
    UPROPERTY()
    URPGSaveGame* CurrentSaveGame;

    UPROPERTY()
    bool bSavingEnabled;

    /** 当前是否在保存数据的过程中*/
    UPROPERTY()
    bool bCurrentlySaving;

    /**在保存数据的过程中有其他的数据保存请求*/
    UPROPERTY()
    bool bPendingSaveRequested;

    /**异步保存发生时被执行*/
    virtual void HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "RPGPlayerControllerBase.generated.h"


/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

    /** 玩家所有的Item */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
    TMap<URPGItem*, FRPGItemData> InventoryData;

    /** 从保存数据加载装备到数组中 */
    UFUNCTION(BlueprintCallable, Category = Inventory)
    bool LoadInventory();
};

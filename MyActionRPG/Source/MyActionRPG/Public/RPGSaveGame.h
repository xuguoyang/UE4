// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */

namespace ERPGSaveGameVersion
{
    enum type
    {
        // Initial version
        Initial,
        // Added Inventory
        AddedInventory,
        // Added ItemData to store count/level
        AddedItemData,

        // -----<new versions must be added before this line>-------------------------------------------------
        VersionPlusOne,
        LatestVersion = VersionPlusOne - 1
    };
}

UCLASS()
class MYACTIONRPG_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    URPGSaveGame()
    {
        SavedDataVersion = ERPGSaveGameVersion::LatestVersion;
    }

    /**道具数据Map，索引是资源ID*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
    TMap<FPrimaryAssetId, FRPGItemData> InventoryData;

    /**插槽数据列表，内容保存资源ID*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
    TMap<FRPGItemSlot, FPrimaryAssetId> SlottedItems;

    /**用户UID*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
    FString UserId;
protected:

    /**保存文档的最后一个版本号*/
    UPROPERTY()
    int32 SavedDataVersion;

    UPROPERTY()
    TArray<FPrimaryAssetId> InventoryItems_DEPRECATED;

    virtual void Serialize(FArchive& Ar) override;
};

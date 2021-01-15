// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGSaveGame.h"

void URPGSaveGame::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    if (Ar.IsLoading() && SavedDataVersion != ERPGSaveGameVersion::LatestVersion)
    {
        if (SavedDataVersion < ERPGSaveGameVersion::AddedItemData)
        {
            for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
            {
                InventoryData.Add(ItemId, FRPGItemData(1, 1));
            }

            InventoryItems_DEPRECATED.Empty();
        }

        SavedDataVersion = ERPGSaveGameVersion::LatestVersion;
    }

}

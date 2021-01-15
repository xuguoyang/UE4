// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGItem.h"

bool URPGItem::IsConsumble() const
{
    if (MaxCount <= 0)
    {
        return true;
    }

    return false;
}

FString URPGItem::GetIdentifierString() const
{
    return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId URPGItem::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(ItemType, GetFName());
}


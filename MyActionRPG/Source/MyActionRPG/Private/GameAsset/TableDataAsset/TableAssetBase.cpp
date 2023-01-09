// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAsset/TableDataAsset/TableAssetBase.h"


FPrimaryAssetId UTableAssetBase::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(AssetType, GetFName());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TableAssetBase.h"


FPrimaryAssetId UTableAssetBase::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(AssetType, GetFName());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAsset/TableDataAsset/TableAssetCommon.h"

UTableAssetCommon* UTableAssetCommon::pInstance = nullptr;
UTableAssetCommon::UTableAssetCommon()
{
    AssetType = URPGAssetManager::CommonDataTableType;
}

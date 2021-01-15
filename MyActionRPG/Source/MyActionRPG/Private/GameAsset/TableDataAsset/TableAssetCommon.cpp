// Fill out your copyright notice in the Description page of Project Settings.


#include "TableAssetCommon.h"

UTableAssetCommon* UTableAssetCommon::pInstance = nullptr;
UTableAssetCommon::UTableAssetCommon()
{
    AssetType = URPGAssetManager::CommonDataTableType;
}

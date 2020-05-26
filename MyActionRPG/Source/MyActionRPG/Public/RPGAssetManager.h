// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "RPGAssetManager.generated.h"

class URPGItem;

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
    virtual void StartInitialLoading() override;

    static const FPrimaryAssetType PotionItemType;
    static const FPrimaryAssetType	SkillItemType;
    static const FPrimaryAssetType	TokenItemType;
    static const FPrimaryAssetType	WeaponItemType;

    /** 获取当前AssetManager对象 */
    static URPGAssetManager& Get();

    /** 加载道具，返回道具对象指针 */
    URPGItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);

};

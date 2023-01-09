// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/AssetManager.h>
#include "RPGAssetManager.generated.h"

class URPGItem;

/**
 * 
 */
UCLASS(Blueprintable)
class MYACTIONRPG_API URPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
    virtual void StartInitialLoading() override;

    static const FPrimaryAssetType PotionItemType;
    static const FPrimaryAssetType	SkillItemType;
    static const FPrimaryAssetType	TokenItemType;
    static const FPrimaryAssetType	WeaponItemType;
    static const FPrimaryAssetType  ControlCharacterType;
    static const FPrimaryAssetType  AttackNpcType;
    static const FPrimaryAssetType  CommonDataTableType;

    /** ��ȡ��ǰAssetManager���� */
    static URPGAssetManager& Get();


    /** ��������DataTable*/
    void LoadAllDataTableAsset();

    /** ͨ��DataTable�ļ���������Դ���سɹ��Ĵ���*/
    FStreamableDelegate CommonDataTableLoadDelegate;

    //void LoadCommonDataTableFinish();


};

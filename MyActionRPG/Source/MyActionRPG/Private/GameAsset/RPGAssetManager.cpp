// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAsset/RPGAssetManager.h"
#include <AbilitySystemGlobals.h>
#include "RPGGameInstanceBase.h"
#include "GameAsset/TableDataAsset/TableAssetCommon.h"
#include "GameAsset/TableDefine/RPGPlayerSeatDataTableRow.h"


const FPrimaryAssetType	URPGAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	URPGAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	URPGAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType	URPGAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType	URPGAssetManager::ControlCharacterType = TEXT("ControlCharacter"); 
const FPrimaryAssetType	URPGAssetManager::AttackNpcType = TEXT("AttackNpc");
const FPrimaryAssetType	URPGAssetManager::CommonDataTableType = TEXT("CommonDataTable");

void URPGAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    UAbilitySystemGlobals::Get().InitGlobalData();

}

URPGAssetManager& URPGAssetManager::Get()
{
    URPGAssetManager* This = Cast<URPGAssetManager>(GEngine->AssetManager);
    if (This)
    {
        return *This;
    }
    else
    {
        UE_LOG(LogActionRPG, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
        return *NewObject<URPGAssetManager>();
    }
}

void URPGAssetManager::LoadAllDataTableAsset()
{
    //CommonDataTableLoadDelegate.BindUObject(this, &URPGAssetManager::LoadCommonDataTableFinish);
    TArray<FName> BundlesArray;
    LoadPrimaryAssetsWithType(CommonDataTableType, BundlesArray, CommonDataTableLoadDelegate, FStreamableManager::AsyncLoadHighPriority);
}

//void URPGAssetManager::LoadCommonDataTableFinish()
//{
//    UDataTable* Table = UTableAssetCommon::Get()->TablePlayerList;
//    if (Table == nullptr)
//    {
//        return;
//    }
//
//    TArray<FRPGPlayerSeatDataTable*> TableRows;
//    Table->GetAllRows(TEXT("LoadCommonDataTableFinish"), TableRows);
//    for (auto& row: TableRows)
//    {
//        row->SpawnPosition;
//    }
//
//}

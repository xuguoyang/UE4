// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstanceBase.h"
#include "RPGSaveGame.h"
#include "GameStageManager.h"
#include "RPGAssetManager.h"
#include "RPGPlayerSeats.h"

URPGGameInstanceBase::URPGGameInstanceBase()
    : SaveSlot(TEXT("SaveGame"))
    , SaveUserIndex(0)
{

}

void URPGGameInstanceBase::Init()
{
    Super::Init();

    StartLoadRoleList();

    URPGAssetManager::Get().CommonDataTableLoadDelegate.BindUObject(this, &URPGGameInstanceBase::LoadCommonDataTableFinish);
    URPGAssetManager::Get().LoadAllDataTableAsset();

    PlayerSeats = NewObject<ARPGPlayerSeats>();
    
}

void URPGGameInstanceBase::LoadComplete(const float LoadTime, const FString& MapName)
{
    // 处理不同阶段的回调
    if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
    {
        // 客户端启动后，默认在登录状态
        GameStageManage::GetInstance().LoadLevelComplete(LoadTime, MapName);
    }
}

URPGSaveGame* URPGGameInstanceBase::GetCurrentSaveGame()
{
    return CurrentSaveGame;
}

void URPGGameInstanceBase::SetSavingEnabled(bool bEnabled)
{
    bSavingEnabled = bEnabled;
}

bool URPGGameInstanceBase::HandleSaveGameLoaded(USaveGame* SaveGameObject)
{
    bool bLoaded = false;
    if (!bSavingEnabled)
    {
        SaveGameObject = nullptr;
    }

    CurrentSaveGame = Cast<URPGSaveGame>(SaveGameObject);
    if (CurrentSaveGame)
    {
        // 确保默认的存档装备加载
        AddDefaultInventory(CurrentSaveGame, false);
        bLoaded = true;
    }
    else
    {
        // This creates it on demand
        CurrentSaveGame = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));
        AddDefaultInventory(CurrentSaveGame, true);
    }

    OnSaveGameLoaded.Broadcast(CurrentSaveGame);
    OnSaveGameLoadedNative.Broadcast(CurrentSaveGame);

    return bLoaded;
}

void URPGGameInstanceBase::GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const
{
    SlotName = SaveSlot;
    UserIndex = SaveUserIndex;
}

bool URPGGameInstanceBase::WriteSaveGame()
{
    if (bSavingEnabled)
    {
        if (bCurrentlySaving)
        {
            bPendingSaveRequested = true;
            return true;
        }

        bCurrentlySaving = true;

        // 执行异步数据保存
        UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &URPGGameInstanceBase::HandleAsyncSave));
        return true;
    }
    return false;
}

UControlRole* URPGGameInstanceBase::GetAssetRole(const FName& AssetName)
{
    return *(DefaultRoles.Find(AssetName));
}

void URPGGameInstanceBase::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
    ensure(bCurrentlySaving);
    bCurrentlySaving = false;

    // 当前有其他的数据保存请求
    if (bPendingSaveRequested)
    {
        // Start another save as we got a request while saving
        bPendingSaveRequested = false;
        WriteSaveGame();
    }
}

void URPGGameInstanceBase::LoadCommonDataTableFinish()
{
    PlayerSeats->InitPlayerSeats();
}

void URPGGameInstanceBase::ControlRoleLoadComplated()
{
    DefaultRoles.Empty();
    TArray<FPrimaryAssetId> PrimaryAssetIdList;
    if (URPGAssetManager::Get().GetPrimaryAssetIdList(URPGAssetManager::ControlCharacterType, PrimaryAssetIdList))
    {
        for (auto& AssetId: PrimaryAssetIdList)
        {
            // 加载对应的UOject对象指针
            UControlRole* pRole = Cast<UControlRole>(URPGAssetManager::Get().GetPrimaryAssetObject(AssetId));
            if (pRole != nullptr)
            {
                DefaultRoles.Add(AssetId.PrimaryAssetName, pRole);
            }
        }
        
    }
}

void URPGGameInstanceBase::StartLoadRoleList()
{
    ControlRoleLoadDelegate.BindUObject(this, &URPGGameInstanceBase::ControlRoleLoadComplated);
    URPGAssetManager::Get().LoadPrimaryAssetsWithType(URPGAssetManager::ControlCharacterType, TArray<FName>(), ControlRoleLoadDelegate);
}

void URPGGameInstanceBase::AddDefaultInventory(URPGSaveGame* SaveGame, bool bRemoveExtra)
{
    if (bRemoveExtra)
    {
        SaveGame->InventoryData.Reset();
    }

    // 将DefaultInventory数据加载到SaveGame.InventoryData中
    for (TPair<FPrimaryAssetId, FRPGItemData>& Pair : DefaultData.DefaultInventory)
    {
        if (!SaveGame->InventoryData.Contains(Pair.Key))
        {
            SaveGame->InventoryData.Add(Pair.Key, Pair.Value);
        }
    }
}

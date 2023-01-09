// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstanceBase.h"
#include "GamePlaying/GameClientStage/GameStageManager.h"
#include "GameAsset/RPGAssetManager.h"
#include "GameSystem/Player/RPGPlayerSeats.h"

URPGGameInstanceBase::URPGGameInstanceBase()
{

}

void URPGGameInstanceBase::Init()
{
    Super::Init();

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


void URPGGameInstanceBase::LoadCommonDataTableFinish()
{
    PlayerSeats->InitPlayerSeats();
}

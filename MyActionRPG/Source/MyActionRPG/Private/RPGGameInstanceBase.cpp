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
    // ����ͬ�׶εĻص�
    if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
    {
        // �ͻ���������Ĭ���ڵ�¼״̬
        GameStageManage::GetInstance().LoadLevelComplete(LoadTime, MapName);
    }
}


void URPGGameInstanceBase::LoadCommonDataTableFinish()
{
    PlayerSeats->InitPlayerSeats();
}

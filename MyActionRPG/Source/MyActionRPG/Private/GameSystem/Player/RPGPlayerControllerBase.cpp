// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerControllerBase.h"
#include "RPGGameInstanceBase.h"
#include "RPGAssetManager.h"
#include "GameStageManager.h"
#include "MainHallGameStateBase.h"
#include "Engine/NetConnection.h"
#include "RPGPlayerStateBase.h"

ARPGPlayerControllerBase::ARPGPlayerControllerBase()
{
    
}

void ARPGPlayerControllerBase::BeginPlay()
{
   Super::BeginPlay();

   //SelectRole(TEXT("StrongestAttacker"));
}   

void ARPGPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPGPlayerControllerBase::SelectRole_Implementation(const FName& RoleName)
{
    ARPGPlayerStateBase* pPlayerState = GetPlayerState<ARPGPlayerStateBase>();
    if (pPlayerState != nullptr)
    {
        pPlayerState->SelectedRoleName = RoleName;
    }
}

void ARPGPlayerControllerBase::LoginSucceed_Implementation()
{
    // 登录成功，客户端切换到角色选择状态
    GameStageManage::GetInstance().SwitchStage(EGameStageBase::EStage_Select);

    ENetMode netMode = GetWorld()->GetNetMode();
    if (netMode == NM_Client)
    {

    }

}
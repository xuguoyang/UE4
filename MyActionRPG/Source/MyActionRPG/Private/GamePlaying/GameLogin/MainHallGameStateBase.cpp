// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlaying/GameLogin/MainHallGameStateBase.h"
#include <Engine/Player.h>
#include "GameSystem/Player/RPGPlayerControllerBase.h"
#include <GameFramework/PlayerState.h>
#include "RPGGameInstanceBase.h"
#include "GameSystem/Player/RPGPlayerSeats.h"

AMainHallGameStateBase::AMainHallGameStateBase():
m_CountDown(10)
{
}

void AMainHallGameStateBase::HandleBeginPlay()
{
    if (GetNetMode() != ENetMode::NM_Client)
    {
        // 初始只有5个玩家
    }

    //PlayerSeats = GetGameInstance<URPGGameInstanceBase>()->PlayerSeats;

    PlayerSeats = GetWorld()->SpawnActor<ARPGPlayerSeats>(ARPGPlayerSeats::StaticClass());
    PlayerSeats->InitPlayerSeats();
}

void AMainHallGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMainHallGameStateBase, m_CountDown); 
    DOREPLIFETIME(AMainHallGameStateBase, PlayerSeats);
}

bool AMainHallGameStateBase::PlayerJion(APlayerController* player)
{
   /* if (!CanJion(player))
    {
        return false;
    }

    FPlayerSeatData& Data = GetEmptySeat();
    if (Data.nPlayerIndex != EPlayerIndex::EPlayer_InVailed)
    {
        Data.pPlayer = player;
        return true;
    }
   
    return false;*/

	bool bSuccess = PlayerSeats->PlayerJion(player);

    return bSuccess;
}

void AMainHallGameStateBase::PlayerLeave(APlayerController* player)
{
    /*for (auto& Elem : m_PlayerList)
    {
        if (Elem.pPlayer == player)
        {
            Elem.RestSeat();
            return;
        }
    }*/

    PlayerSeats->PlayerLeave(player);
}

bool AMainHallGameStateBase::CanJion(APlayerController* player)
{
    

    return true;
}

void AMainHallGameStateBase::OnRep_CountDown()
{
	DelegateOnGameCountDownChanged.Broadcast();
}
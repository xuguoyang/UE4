// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameStateBase.h"
#include "RPGGameInstanceBase.h"

void ARPGGameStateBase::HandleBeginPlay()
{
   //PlayerSeats = GetGameInstance<URPGGameInstanceBase>()->PlayerSeats;
   /* if (PlayerSeats != nullptr)
    {
        PlayerSeats->GetEmptySeat();
        PlayerSeats->m_PlayerArray.Empty();
    }*/
}

void ARPGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPGGameStateBase, PlayerSeats);
}

void ARPGGameStateBase::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    GameState_Started();
}

void ARPGGameStateBase::GameState_Started_Implementation()
{

}

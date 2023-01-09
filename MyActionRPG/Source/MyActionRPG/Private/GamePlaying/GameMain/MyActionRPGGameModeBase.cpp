// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlaying/GameMain/MyActionRPGGameModeBase.h"
#include "GameSystem/Player/RPGPlayerControllerBase.h"
#include "GamePlaying/GameMain/RPGGameStateBase.h"
#include "RPGGameInstanceBase.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerStart.h>
#include "GameAsset/TableDefine/RPGRoleDataTableRow.h"
#include "GameAsset/TableDefine/RPGPlayerSeatDataTableRow.h"
#include "GameSystem/Player/RPGPlayerStateBase.h"

AMyActionRPGGameModeBase::AMyActionRPGGameModeBase()
{
    // ����GameState��PlayerController��ʹ�õ�UClass����
    //GameStateClass =
    GameStateClass = ARPGGameStateBase::StaticClass();
    PlayerControllerClass = ARPGPlayerControllerBase::StaticClass();
}

void AMyActionRPGGameModeBase::StartPlay()
{
    Super::StartPlay();
    AGameModeBase::StartPlay();

    StartMatch();
}

void AMyActionRPGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    // ��ʼ�淨����������ٽ�����Ϸ
    if (GetMatchState() == MatchState::InProgress)
    {
        ErrorMessage = TEXT("Game Already Started, cannot enter the game!");
    }
}

void AMyActionRPGGameModeBase::RestartPlayer(AController* NewPlayer)
{
    ARPGPlayerStateBase* pPlayerState = NewPlayer->GetPlayerState<ARPGPlayerStateBase>();
    if (pPlayerState == nullptr)
    {
        return;
    }

    // ��ȡplayer��Ӧ������
    FRPGRoleDataTableRow* RoleDataRow = UTableAssetCommon::Get()->TableRoleList->FindRow<FRPGRoleDataTableRow>(pPlayerState->SelectedRoleName, pPlayerState->SelectedRoleName.ToString());
    FRPGPlayerSeatDataTableRow* PlayerSeatRow = UTableAssetCommon::Get()->TablePlayerList->FindRow<FRPGPlayerSeatDataTableRow>(pPlayerState->SeatName, pPlayerState->SeatName.ToString());

    // �������ô���StartSpot��character
    if (nullptr == RoleDataRow || nullptr == PlayerSeatRow)
    {
        return;
    }

    NewPlayer->StartSpot = GetWorld()->SpawnActor<APlayerStart>(PlayerSeatRow->SpawnPosition, PlayerSeatRow->SpawnRotation);
    APawn* Character = GetWorld()->SpawnActor<APawn>(RoleDataRow->RoleActor.Get(), PlayerSeatRow->SpawnPosition, PlayerSeatRow->SpawnRotation);
    NewPlayer->SetPawn(Character);

    Super::RestartPlayer(NewPlayer);
}

void AMyActionRPGGameModeBase::PostSeamlessTravel()
{
    Super::PostSeamlessTravel();

    // ��PlayerSeatsͬ�����µ�map
    GetWorld()->PersistentLevel->Actors.FindItemByClass<ARPGPlayerSeats>(&GetGameState<ARPGGameStateBase>()->PlayerSeats);

}

void AMyActionRPGGameModeBase::Logout(AController* Exiting)
{
    ARPGGameStateBase* pRPGGameState = GetGameState<ARPGGameStateBase>();
    if (pRPGGameState == nullptr)
    {
        return;
    }

    if (pRPGGameState->PlayerSeats == nullptr)
    {
        return;
    }

    pRPGGameState->PlayerSeats->PlayerLeave(Cast<APlayerController>(Exiting));
}

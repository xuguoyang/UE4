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
    // 设置GameState、PlayerController所使用的UClass对象
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
    // 开始玩法后不允许玩家再进入游戏
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

    // 读取player对应的配置
    FRPGRoleDataTableRow* RoleDataRow = UTableAssetCommon::Get()->TableRoleList->FindRow<FRPGRoleDataTableRow>(pPlayerState->SelectedRoleName, pPlayerState->SelectedRoleName.ToString());
    FRPGPlayerSeatDataTableRow* PlayerSeatRow = UTableAssetCommon::Get()->TablePlayerList->FindRow<FRPGPlayerSeatDataTableRow>(pPlayerState->SeatName, pPlayerState->SeatName.ToString());

    // 根据配置创建StartSpot和character
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

    // 将PlayerSeats同步到新的map
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

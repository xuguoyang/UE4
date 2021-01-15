// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginMainHallModeBase.h"
#include "RPGPlayerControllerBase.h"
#include "MainHallGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include <assert.h>

void ALoginMainHallModeBase::StartPlay()
{
    Super::StartPlay();
    AGameModeBase::StartPlay();

    m_pGameState = GetGameState<AMainHallGameStateBase>();
    assert(m_pGameState);
}

void ALoginMainHallModeBase::CountDownFuction()
{
    --m_pGameState->m_CountDown;

    // 倒计时小于0时，停止倒计时
    if (m_pGameState->m_CountDown < 0)
    {
        m_pGameState->m_CountDown = 0;
        EndCountDown();
    }
}

void ALoginMainHallModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    // 玩家登陆成功
    AMainHallGameStateBase* pMainHallGameStateBase = Cast<AMainHallGameStateBase>(GameState);
    if (!pMainHallGameStateBase->PlayerSeats->HasEmptySeat())
    {
        ErrorMessage = TEXT("Cannot jion,No empty seat!");
    }
}

APlayerController* ALoginMainHallModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    APlayerController* pController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

    ARPGPlayerControllerBase* pRPGController = Cast<ARPGPlayerControllerBase>(pController);
    if (pRPGController)
    {
        ChangeName(pRPGController, UGameplayStatics::ParseOption(Options, TEXT("username")), true);
    }

    return pController; 
}

void ALoginMainHallModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (GetNetMode() == NM_Standalone)
    {
        return;
    }

    ARPGPlayerControllerBase* pARPGPlayerControllerBase = Cast<ARPGPlayerControllerBase>(NewPlayer);
    if (!pARPGPlayerControllerBase) return;

    // 玩家登陆成功
    AMainHallGameStateBase* pMainHallGameStateBase = Cast<AMainHallGameStateBase>(GameState);
    if (pMainHallGameStateBase != NULL)
    {
        // 需要至少有一个玩家进入房间才能开始倒计时
        if (pMainHallGameStateBase->PlayerJion(NewPlayer) && !GetWorldTimerManager().IsTimerActive(m_CountDownTimerHandle))
        {
            StartCountDown();
        }
    }

    // 通知客户端登录成功
    pARPGPlayerControllerBase->LoginSucceed();
}

void ALoginMainHallModeBase::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
    Super::GetSeamlessTravelActorList(bToTransition, ActorList);

    
    ActorList.Add(GetGameState<AMainHallGameStateBase>()->PlayerSeats);
}

void ALoginMainHallModeBase::Logout(AController* Exiting)
{
    GetGameState<AMainHallGameStateBase>()->PlayerSeats->PlayerLeave(Cast<APlayerController>(Exiting));
}

void ALoginMainHallModeBase::StartCountDown()
{
    m_pGameState->m_CountDown = 30;
    GetWorldTimerManager().SetTimer(m_CountDownTimerHandle, this, &ALoginMainHallModeBase::CountDownFuction, true, 1.0f, -1.0f);
}

void ALoginMainHallModeBase::EndCountDown()
{
    GetWorldTimerManager().ClearTimer(m_CountDownTimerHandle);

    // 传送玩家到玩法场景,无缝传送
    bUseSeamlessTravel = true;
    GetWorld()->ServerTravel(TEXT("/Game/Maps/ActionRPG_P"));
}

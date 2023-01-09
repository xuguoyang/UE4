// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlaying/GameLogin/LoginMainHallModeBase.h"
#include "GameSystem/Player/RPGPlayerControllerBase.h"
#include "GamePlaying/GameLogin/MainHallGameStateBase.h"
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
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

    // ����ʱС��0ʱ��ֹͣ����ʱ
    if (m_pGameState->m_CountDown < 0)
    {
        m_pGameState->m_CountDown = 0;
        EndCountDown();
    }
}

void ALoginMainHallModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    // ��ҵ�½�ɹ�
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

    // ��ҵ�½�ɹ�
    AMainHallGameStateBase* pMainHallGameStateBase = Cast<AMainHallGameStateBase>(GameState);
    if (pMainHallGameStateBase != NULL)
    {
        // ��Ҫ������һ����ҽ��뷿����ܿ�ʼ����ʱ
        if (pMainHallGameStateBase->PlayerJion(NewPlayer) && !GetWorldTimerManager().IsTimerActive(m_CountDownTimerHandle))
        {
            StartCountDown();
        }
    }

    // ֪ͨ�ͻ��˵�¼�ɹ�
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

    // ������ҵ��淨����,�޷촫��
    bUseSeamlessTravel = true;
    GetWorld()->ServerTravel(LevelPath.FilePath);
}

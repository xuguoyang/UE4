// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LoginMainHallModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ALoginMainHallModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
    virtual void StartPlay() override;

    // 倒计时定时器函数
    void CountDownFuction();

protected:
    UPROPERTY(EditAnywhere, meta=(FilePathFilter = "umap", LongPackageName))
    FFilePath LevelPath;

private:
    virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

    virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList) override;

    virtual void Logout(AController* Exiting) override;
private:
    // 开始倒计时
    void StartCountDown();

    // 倒计时结束
    void EndCountDown();

private:
    FTimerHandle m_CountDownTimerHandle;

    UPROPERTY()
    class AMainHallGameStateBase* m_pGameState;
};

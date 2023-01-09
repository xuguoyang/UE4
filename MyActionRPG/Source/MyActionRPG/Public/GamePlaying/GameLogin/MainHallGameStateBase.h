// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameSystem/Player/RPGPlayerSeats.h"
#include "MainHallGameStateBase.generated.h"

/**
 * 
 */

enum EPlayerIndex:int8
{
    EPlayer_InVailed = -1,
    EPlayer_0        = 0,
    EPlayer_1        = 1,
    EPlayer_2        = 2,
    EPlayer_3        = 3,
    EPlayer_4        = 4,
    EPlayer_Max,
};


//倒计时改变
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCountDownChangedDelegate);

UCLASS()
class MYACTIONRPG_API AMainHallGameStateBase : public AGameState
{
	GENERATED_BODY()
public:
    // 游戏启动倒计时
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CountDown)
    int32  m_CountDown;

    /** 玩家座位对象*/
    UPROPERTY(BlueprintReadOnly, Replicated)
    class  ARPGPlayerSeats* PlayerSeats;

public:
    AMainHallGameStateBase();

    virtual void HandleBeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    // 大厅中加入玩家
    bool PlayerJion(APlayerController* player);

    // 玩家离开大厅
    void PlayerLeave(APlayerController* player);

    // 玩家是否可以加入
    bool CanJion(APlayerController* player);

	UFUNCTION()
	void OnRep_CountDown();

	//倒计时改变回调通知蓝图委托
	UPROPERTY(BlueprintAssignable)
	FGameCountDownChangedDelegate DelegateOnGameCountDownChanged;
};

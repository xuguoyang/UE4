// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameAsset/TableDefine/RPGPlayerSeatDataTableRow.h"
#include "GameFramework/Actor.h"
#include "RPGPlayerSeats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSeat
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FName ConfigID;                     // 当前位置对应的配置ID

    UPROPERTY()
    int8 nPlayerIndex;                  // 玩家索引

    UPROPERTY(BlueprintReadOnly)                         // 当前玩家对象
    class ARPGPlayerStateBase* pPlayer;

    void RestSeat()
    {
        pPlayer = nullptr;
    }

	FPlayerSeat():pPlayer(nullptr)
	{

	}
};

//玩家队列改变
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerArrayChangedDelegate);

UCLASS()
class MYACTIONRPG_API ARPGPlayerSeats : public AActor
{
	GENERATED_BODY()

public:
    ARPGPlayerSeats();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

    void InitPlayerSeats();

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerArray)
    TArray<FPlayerSeat> m_PlayerArray;

    /** 玩家进入游戏*/
    bool PlayerJion(APlayerController* player);

    /** 玩家离开游戏*/
    void PlayerLeave(APlayerController* player);

    /** 玩家是否可以进入*/
    bool CanJion(APlayerController* player);

    FPlayerSeat& GetEmptySeat();

    bool HasEmptySeat();

	//玩家队列改变回调通知蓝图委托
	UPROPERTY(BlueprintAssignable)
	FPlayerArrayChangedDelegate DelegateOnPlayerArrayChanged;

protected:
    UFUNCTION()
    void OnRep_PlayerArray();

    virtual void Destroyed() override;
};

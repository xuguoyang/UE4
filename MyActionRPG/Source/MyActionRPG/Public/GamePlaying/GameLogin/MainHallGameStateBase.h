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


//����ʱ�ı�
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCountDownChangedDelegate);

UCLASS()
class MYACTIONRPG_API AMainHallGameStateBase : public AGameState
{
	GENERATED_BODY()
public:
    // ��Ϸ��������ʱ
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CountDown)
    int32  m_CountDown;

    /** �����λ����*/
    UPROPERTY(BlueprintReadOnly, Replicated)
    class  ARPGPlayerSeats* PlayerSeats;

public:
    AMainHallGameStateBase();

    virtual void HandleBeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    // �����м������
    bool PlayerJion(APlayerController* player);

    // ����뿪����
    void PlayerLeave(APlayerController* player);

    // ����Ƿ���Լ���
    bool CanJion(APlayerController* player);

	UFUNCTION()
	void OnRep_CountDown();

	//����ʱ�ı�ص�֪ͨ��ͼί��
	UPROPERTY(BlueprintAssignable)
	FGameCountDownChangedDelegate DelegateOnGameCountDownChanged;
};

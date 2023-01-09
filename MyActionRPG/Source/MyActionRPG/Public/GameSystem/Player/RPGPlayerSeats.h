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
    FName ConfigID;                     // ��ǰλ�ö�Ӧ������ID

    UPROPERTY()
    int8 nPlayerIndex;                  // �������

    UPROPERTY(BlueprintReadOnly)                         // ��ǰ��Ҷ���
    class ARPGPlayerStateBase* pPlayer;

    void RestSeat()
    {
        pPlayer = nullptr;
    }

	FPlayerSeat():pPlayer(nullptr)
	{

	}
};

//��Ҷ��иı�
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

    /** ��ҽ�����Ϸ*/
    bool PlayerJion(APlayerController* player);

    /** ����뿪��Ϸ*/
    void PlayerLeave(APlayerController* player);

    /** ����Ƿ���Խ���*/
    bool CanJion(APlayerController* player);

    FPlayerSeat& GetEmptySeat();

    bool HasEmptySeat();

	//��Ҷ��иı�ص�֪ͨ��ͼί��
	UPROPERTY(BlueprintAssignable)
	FPlayerArrayChangedDelegate DelegateOnPlayerArrayChanged;

protected:
    UFUNCTION()
    void OnRep_PlayerArray();

    virtual void Destroyed() override;
};

#include "GameSystem/Player/RPGPlayerSeats.h"
#include "GameAsset/TableDataAsset/TableAssetCommon.h"
#include "GameAsset/TableDefine/RPGPlayerSeatDataTableRow.h"


ARPGPlayerSeats::ARPGPlayerSeats()
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

void ARPGPlayerSeats::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARPGPlayerSeats, m_PlayerArray);
}

void ARPGPlayerSeats::InitPlayerSeats()
{
    UDataTable* pTablePlayerList = UTableAssetCommon::Get()->TablePlayerList;
    if (pTablePlayerList == nullptr)
    {
        return;
    }

    int8 index = 0;
    pTablePlayerList->ForeachRow<FRPGPlayerSeatDataTableRow>(TEXT("URPGPlayerSeats::InitPlayerSeats"), [this, &index](const FName& Key, const FRPGPlayerSeatDataTableRow& Value)
    {
        FPlayerSeat PlayerSeat;
        PlayerSeat.ConfigID = Key;
        PlayerSeat.nPlayerIndex = index;
        m_PlayerArray.Add(PlayerSeat);
        ++index;
    });
}

bool ARPGPlayerSeats::PlayerJion(APlayerController* player)
{
    if (!CanJion(player))
    {
        return false;
    }

    FPlayerSeat& Data = GetEmptySeat();
    if (Data.nPlayerIndex != -1)
    {
        Data.pPlayer = player->GetPlayerState<ARPGPlayerStateBase>();
        if (Data.pPlayer != nullptr)
        {
            Data.pPlayer->SeatName = Data.ConfigID;
            return true;
        } 
    }

    return false;
}

void ARPGPlayerSeats::PlayerLeave(APlayerController* player)
{
    for (auto& Elem : m_PlayerArray)
    {
        if (Elem.pPlayer == player->PlayerState)
        {
            Elem.RestSeat();
            return;
        }
    }
}

bool ARPGPlayerSeats::CanJion(APlayerController* player)
{
    for (auto& Elem : m_PlayerArray)
    {
        if (Elem.pPlayer == player->PlayerState)
        {
            return false;
        }
    }

    return true;
}

FPlayerSeat& ARPGPlayerSeats::GetEmptySeat()
{
    for (auto& Elem : m_PlayerArray)
    {
        if (Elem.pPlayer == nullptr)
        {
            return Elem;
        }
    }

    static FPlayerSeat InvaildData;
    InvaildData.nPlayerIndex = -1;
    return InvaildData;
}

bool ARPGPlayerSeats::HasEmptySeat()
{
    FPlayerSeat& Data = GetEmptySeat();
    if (Data.nPlayerIndex != -1) return true;

    return false;
}

void ARPGPlayerSeats::OnRep_PlayerArray()
{
	//玩家队列发生改变通知蓝图
	DelegateOnPlayerArrayChanged.Broadcast();
}

void ARPGPlayerSeats::Destroyed()
{
    Super::Destroyed();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Player/RPGPlayerStateBase.h"

void ARPGPlayerStateBase::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);
    ARPGPlayerStateBase* pARPGPlayerStateBase = Cast<ARPGPlayerStateBase>(PlayerState);
    if (pARPGPlayerStateBase == nullptr)
    {
        return;
    }

    // 跨场景拷贝数据
    pARPGPlayerStateBase->SelectedRoleName = SelectedRoleName;
    pARPGPlayerStateBase->SeatName = SeatName;

}

void ARPGPlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPGPlayerStateBase, SelectedRoleName); 
    DOREPLIFETIME(ARPGPlayerStateBase, SeatName);
}

void ARPGPlayerStateBase::OnRep_SelectedRole()
{
	//玩家选的角色发生变化通知蓝图
	DelegateRoleSelected.Broadcast(this);
}


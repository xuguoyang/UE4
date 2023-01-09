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

    // �糡����������
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
	//���ѡ�Ľ�ɫ�����仯֪ͨ��ͼ
	DelegateRoleSelected.Broadcast(this);
}


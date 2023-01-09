// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedRoleDelegate, ARPGPlayerStateBase*, pStateBase);

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SelectedRole)
    FName SelectedRoleName;                // ��ǰѡ��Ľ�ɫ����

    UPROPERTY(Replicated)
    FName SeatName;                        // ��ǰ���λ��ConfigID

	//ѡ���ɫ֪ͨ��ͼί�лص�
	UPROPERTY(BlueprintAssignable)
	FSelectedRoleDelegate DelegateRoleSelected;
	ARPGPlayerStateBase* pStateBase;

private:
    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
private:
    UFUNCTION()
    void OnRep_SelectedRole();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
    UPROPERTY(ReplicatedUsing = OnRep_SelectedRole)
    FName SelectedRoleName;                // 当前选择的角色索引

    UPROPERTY(Replicated)
    FName SeatName;                        // 当前玩家位置ConfigID
private:
    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
private:
    UFUNCTION()
    void OnRep_SelectedRole();
};

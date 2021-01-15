// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RPGGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGGameStateBase : public AGameState
{
	GENERATED_BODY()
	
public:
    /** 玩家座位对象*/
    UPROPERTY(Replicated)
    class  ARPGPlayerSeats* PlayerSeats;

    virtual void HandleBeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};

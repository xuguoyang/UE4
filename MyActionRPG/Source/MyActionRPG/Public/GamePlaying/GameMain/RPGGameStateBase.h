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
    /** �����λ����*/
    UPROPERTY(Replicated)
    class  ARPGPlayerSeats* PlayerSeats;

protected:
    virtual void HandleBeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    virtual void HandleMatchHasStarted() override;

    UFUNCTION(BlueprintNativeEvent, Category = GameState)
    void GameState_Started();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyActionRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API AMyActionRPGGameModeBase : public AGameMode
{
	GENERATED_BODY() 
public:
    AMyActionRPGGameModeBase();

protected:
    virtual void StartPlay() override;

    virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

    virtual void RestartPlayer(AController* NewPlayer) override;

    virtual void PostSeamlessTravel() override;

    virtual void Logout(AController* Exiting) override;

};

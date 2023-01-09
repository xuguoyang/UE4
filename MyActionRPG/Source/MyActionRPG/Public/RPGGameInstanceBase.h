// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGType.h"
#include "Engine/GameInstance.h"
#include "GameSystem/Roles/RPGCharacterBase.h"
#include "RPGGameInstanceBase.generated.h"


UCLASS()
class MYACTIONRPG_API URPGGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
    URPGGameInstanceBase();

    virtual void Init() override;
    
    // 关卡加载完成的回调
    virtual void LoadComplete(const float LoadTime, const FString& MapName) override;

#pragma region rolelist
public:
    UPROPERTY()
    class  ARPGPlayerSeats* PlayerSeats;



   
#pragma endregion rolelist

protected:
    void LoadCommonDataTableFinish();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "RPGPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
    ARPGPlayerControllerBase();

    virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;



// 服务器向客户端发送的RPC
public:
    /** 登录成功通知客户端*/
    UFUNCTION(Client, Reliable)
    void LoginSucceed();

// 客户端向服务器发送消息
public:
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void SelectRole(const FName& RoleName);
};

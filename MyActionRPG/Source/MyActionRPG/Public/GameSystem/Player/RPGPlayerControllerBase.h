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

	virtual void OnRep_Pawn();

	UFUNCTION(BlueprintCallable, Category = Pawn)
	class APlayerCharacterBase* GetPlayerCharacter() const;

// 服务器向客户端发送的RPC
public:
    /** 登录成功通知客户端*/
    UFUNCTION(Client, Reliable)
    void LoginSucceed();

// 客户端向服务器发送消息
public:
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void SelectRole(const FName& RoleName);


public:
	//道具背包系统
#pragma region Item
	uint8 bHasPlayerStart : 1;

	// 客户端已经准备好，发送给服务器
	UFUNCTION(Server, Reliable, Category = "Battle")
	void ServerPlayerStart();

	UFUNCTION(Reliable, Client)
	void ClientClearAllItem(class UGridContainerComponent* GridContainerComponent);

	UFUNCTION(Reliable, Client)
	void ClientAddItem(class UGridContainerComponent* GridContainerComponent, enum EContainerType ContainerType, FItemNetInfo itemInfo, bool bShowNotification);

	UFUNCTION(Reliable, Client)
	void ClientRemoveItem(class UGridContainerComponent* GridContainerComponent, enum EContainerType ContainerType, int32 slotIndex, bool bShowNotification);

	UFUNCTION(Reliable, Client)
	void ClientUpdateItem(class UGridContainerComponent* GridContainerComponent, enum EContainerType ContainerType, FItemNetInfo itemInfo, bool bShowNotification);

	//测试发道具
	UFUNCTION(Server, Reliable)
	void ServerGMAddItem(int32 itemId, int32 AmountOverride = 1);

	//测试丢弃道具
	UFUNCTION(Server, Reliable)
	void ServerGMRemoveItem(int32 soltIndex, int32 AmountOverride = 1);

	//测试清空
	UFUNCTION(Server, Reliable)
	void ServerGMClearAllItem();

	//测试交换道具
	UFUNCTION(Server, Reliable)
	void ServerGMExchangeItem(int32 SourceSlot, int32 TargetSlot);

	//测试移动道具
	UFUNCTION(Server, Reliable)
	void ServerGMMoveItem(int32 SourceSlot, int32 MoveAmount, int32 TargetSlot);
};

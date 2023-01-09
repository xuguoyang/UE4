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

// ��������ͻ��˷��͵�RPC
public:
    /** ��¼�ɹ�֪ͨ�ͻ���*/
    UFUNCTION(Client, Reliable)
    void LoginSucceed();

// �ͻ����������������Ϣ
public:
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void SelectRole(const FName& RoleName);


public:
	//���߱���ϵͳ
#pragma region Item
	uint8 bHasPlayerStart : 1;

	// �ͻ����Ѿ�׼���ã����͸�������
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

	//���Է�����
	UFUNCTION(Server, Reliable)
	void ServerGMAddItem(int32 itemId, int32 AmountOverride = 1);

	//���Զ�������
	UFUNCTION(Server, Reliable)
	void ServerGMRemoveItem(int32 soltIndex, int32 AmountOverride = 1);

	//�������
	UFUNCTION(Server, Reliable)
	void ServerGMClearAllItem();

	//���Խ�������
	UFUNCTION(Server, Reliable)
	void ServerGMExchangeItem(int32 SourceSlot, int32 TargetSlot);

	//�����ƶ�����
	UFUNCTION(Server, Reliable)
	void ServerGMMoveItem(int32 SourceSlot, int32 MoveAmount, int32 TargetSlot);
};

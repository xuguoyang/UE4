// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Player/RPGPlayerControllerBase.h"
#include "RPGGameInstanceBase.h"
#include "GameAsset/RPGAssetManager.h"
#include "GamePlaying/GameClientStage/GameStageManager.h"
#include "GamePlaying/GameLogin/MainHallGameStateBase.h"
#include <Engine/NetConnection.h>
#include "GameSystem/Player/RPGPlayerStateBase.h"
#include "GameSystem/Roles/PlayerCharacterBase.h"
#include "GameSystem/Item/Component/GridContainerComponent.h"

ARPGPlayerControllerBase::ARPGPlayerControllerBase()
{
    
}

void ARPGPlayerControllerBase::BeginPlay()
{
   Super::BeginPlay();

   //SelectRole(TEXT("StrongestAttacker"));
}   

void ARPGPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPGPlayerControllerBase::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		if (bHasPlayerStart)
		{
			return;
		}
		bHasPlayerStart = true;
		ServerPlayerStart();
	}
}

class APlayerCharacterBase* ARPGPlayerControllerBase::GetPlayerCharacter() const
{
	if (GetPawn())
	{
		APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetPawn());
		if (PlayerCharacter)
		{
			return PlayerCharacter;
		}
	}
	return nullptr;
}

void ARPGPlayerControllerBase::SelectRole_Implementation(const FName& RoleName)
{
    ARPGPlayerStateBase* pPlayerState = GetPlayerState<ARPGPlayerStateBase>();
    if (pPlayerState != nullptr)
    {
        pPlayerState->SelectedRoleName = RoleName;
    }
}

void ARPGPlayerControllerBase::ServerPlayerStart_Implementation()
{
	APlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	if (nullptr == PlayerCharacter->GridContainerComponent)
	{
		return;
	}
	PlayerCharacter->GridContainerComponent->ReplicateInitializeData();

	if (!bHasPlayerStart)
	{
		//第一次登录
	}

	bHasPlayerStart = true;
}

void ARPGPlayerControllerBase::ClientClearAllItem_Implementation(class UGridContainerComponent* GridContainerComponent)
{
	if (GridContainerComponent)
	{
		GridContainerComponent->ClearAllItem();
	}
}

void ARPGPlayerControllerBase::ClientAddItem_Implementation(class UGridContainerComponent* GridContainerComponent, enum EContainerType ContainerType, FItemNetInfo itemInfo, bool bShowNotification)
{
	if (GetLocalRole() == ROLE_Authority || nullptr == GridContainerComponent)
	{
		return;
	}

	GridContainerComponent->AddItemByNetInfo(ContainerType, itemInfo, bShowNotification);
}

void ARPGPlayerControllerBase::ClientRemoveItem_Implementation(class UGridContainerComponent* GridContainerComponent, enum EContainerType ContainerType, int32 slotIndex, bool bShowNotification)
{
	if (GetLocalRole() == ROLE_Authority || nullptr == GridContainerComponent)
	{
		return;
	}

	if (slotIndex >= 0)
	{
		GridContainerComponent->RemoveItemBySlot(ContainerType, slotIndex, EItemChangeReason::InvalidReason, -1, bShowNotification);
	}
}

void ARPGPlayerControllerBase::ClientUpdateItem_Implementation(class UGridContainerComponent* GridContainerComponent, enum EContainerType ContainerType, FItemNetInfo itemInfo, bool bShowNotification)
{
	if (GetLocalRole() == ROLE_Authority || nullptr == GridContainerComponent)
	{
		return;
	}

	if (itemInfo.SlotIndex >= 0)
	{
		GridContainerComponent->UpdateItemBySlot(ContainerType, itemInfo.SlotIndex, itemInfo, EItemChangeReason::InvalidReason, bShowNotification);
	}
}

void ARPGPlayerControllerBase::ServerGMAddItem_Implementation(int32 itemId, int32 AmountOverride /*= 1*/)
{
	APlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	if (nullptr != PlayerCharacter->GridContainerComponent)
	{
		PlayerCharacter->GridContainerComponent->AddItemByItemId(EContainerType::CONTAINER_TYPE_BAG, itemId, EItemChangeReason::GM, AmountOverride);
	}
}

void ARPGPlayerControllerBase::ServerGMRemoveItem_Implementation(int32 soltIndex, int32 AmountOverride /*= 1*/)
{
	APlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	if (nullptr != PlayerCharacter->GridContainerComponent)
	{
		PlayerCharacter->GridContainerComponent->RemoveItemBySlot(EContainerType::CONTAINER_TYPE_BAG, soltIndex, EItemChangeReason::GM, AmountOverride);
	}
}

void ARPGPlayerControllerBase::ServerGMClearAllItem_Implementation()
{
	APlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	if (nullptr != PlayerCharacter->GridContainerComponent)
	{
		PlayerCharacter->GridContainerComponent->ClearAllItem(EContainerType::CONTAINER_TYPE_BAG);
	}
}

void ARPGPlayerControllerBase::ServerGMExchangeItem_Implementation(int32 SourceSlot, int32 TargetSlot)
{
	APlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	if (nullptr != PlayerCharacter->GridContainerComponent)
	{
		PlayerCharacter->GridContainerComponent->ExchangeItem(EContainerType::CONTAINER_TYPE_BAG, SourceSlot, EContainerType::CONTAINER_TYPE_BAG, TargetSlot, false);
	}
}

void ARPGPlayerControllerBase::ServerGMMoveItem_Implementation(int32 SourceSlot, int32 MoveAmount, int32 TargetSlot)
{
	APlayerCharacterBase* PlayerCharacter = GetPlayerCharacter();
	if (nullptr == PlayerCharacter)
	{
		return;
	}

	if (nullptr != PlayerCharacter->GridContainerComponent)
	{
		PlayerCharacter->GridContainerComponent->MoveItem(EContainerType::CONTAINER_TYPE_BAG, SourceSlot, MoveAmount, EContainerType::CONTAINER_TYPE_BAG, TargetSlot, false);
	}
}

void ARPGPlayerControllerBase::LoginSucceed_Implementation()
{
    // 登录成功，客户端切换到角色选择状态
    GameStageManage::GetInstance().SwitchStage(EGameStageBase::EStage_Select);

    ENetMode netMode = GetWorld()->GetNetMode();
    if (netMode == NM_Client)
    {

    }

}
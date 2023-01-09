// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Item/Component/GridContainerComponent.h"
#include "GameSystem/Item/BaseContainer.h"
#include "GameSystem/Player/RPGPlayerControllerBase.h"
#include "GameSystem/Item/BaseItem.h"
#include "GameSystem/Roles/PlayerCharacterBase.h"

// Sets default values for this component's properties
UGridContainerComponent::UGridContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	bLoadFromDataStore = false;
}


// Called when the game starts
void UGridContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGridContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ClientMessageLoop();
}

void UGridContainerComponent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	if (Ar.IsSaveGame() && Ar.IsLoading())
	{
		bLoadFromDataStore = true;
	}
}

void UGridContainerComponent::InitializeComponent()
{
	InitContainerData();
}

ENetRole UGridContainerComponent::GetLocalRole()
{
	AActor* Actor = GetOwner();
	if (nullptr == Actor)
	{
		return ROLE_None;
	}

	return Actor->GetLocalRole();

}

class APlayerCharacterBase* UGridContainerComponent::GetPlayer()
{
	if (!OwnerPlayerCharacter.IsValid())
	{
		OwnerPlayerCharacter = Cast<APlayerCharacterBase>(GetOwner());
	}

	return OwnerPlayerCharacter.Get();
}

class ARPGPlayerControllerBase* UGridContainerComponent::GetOwnerControlller()
{
	if (!OwnerPlayerController.IsValid())
	{
		APlayerCharacterBase* BaseCharacter = GetPlayer();
		if (BaseCharacter)
		{
			OwnerPlayerController = Cast<ARPGPlayerControllerBase>(BaseCharacter->GetController());
		}
		else
		{
			return NULL;
		}
	}

	return OwnerPlayerController.Get();
}

TArray<class ARPGPlayerControllerBase*> UGridContainerComponent::GetAllViewPlayerController()
{
	TArray<ARPGPlayerControllerBase*> AllPlayerControllers;
	if (OwnerPlayerController.Get())
	{
		AllPlayerControllers.AddUnique(OwnerPlayerController.Get());
	}

	return AllPlayerControllers;
}

void UGridContainerComponent::InitContainerData()
{
	if (bLoadFromDataStore)
	{
		for (auto& Elem : ContainerMap)
		{
			if (ContainerIndexMap.Find(Elem.Key))
			{
				ContainerIndexMap[Elem.Key]->InitContainer(this);
			}
			else {
				UBaseContainer* container = NewObject<UBaseContainer>(GetOwner(), Elem.Value);
				if (nullptr != container)
				{
					ContainerIndexMap.Add(Elem.Key, container);
					container->InitContainer(this);
				}
			}
		}
	}
	else
	{
		for (auto& Elem : ContainerMap)
		{
			UBaseContainer* container = NewObject<UBaseContainer>(GetOwner(), Elem.Value);
			if (nullptr != container)
			{
				ContainerIndexMap.Add(Elem.Key, container);
				container->InitContainer(this);
			}
		}
	}
}

void UGridContainerComponent::ReplicateInitializeData()
{
	class ARPGPlayerControllerBase* PlayerController = GetOwnerControlller();
	if (nullptr != PlayerController && !PlayerController->IsLocalController())
	{
		for (auto& Elem : ContainerIndexMap)
		{
			UBaseContainer* container = Elem.Value;
			if (nullptr != container)
			{
				FItemNetInfo ItemNetInfo;
				AddClientMessageQueue(PlayerController, container->ContainerType, ENotifyItemType::NOTIFY_INVENTORY_CLEARALL, ItemNetInfo, -1);

				for (UBaseItem* anItem : container->Items)
				{
					if (anItem)
					{
						NotifyClientItemStatus(container->ContainerType, ENotifyItemType::NOTIFY_ITEM_ADD, anItem, -1);
					}
				}
			}
		}
	}
}

void UGridContainerComponent::AddClientMessageQueue(class ARPGPlayerControllerBase* PlayerController, enum EContainerType ContainerType, enum ENotifyItemType NotifyType, FItemNetInfo& ItemNetInfo, int32 Value, bool bShowNotification /*= false*/)
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	FItemMsg* Message = new FItemMsg();
	Message->PlayerController = PlayerController;
	Message->ContainerType = ContainerType;
	Message->NotifyType = NotifyType;
	Message->ItemNetInfo = ItemNetInfo;
	Message->Value = Value;
	Message->bShowNotification = bShowNotification;

	ClientMessageQueue.Enqueue(Message);
}

void UGridContainerComponent::NotifyClientItemStatus(enum EContainerType ContainerType, enum ENotifyItemType NotifyType, class UBaseItem* anItem, int32 Value, bool bShowNotification /*= false*/)
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	FItemNetInfo ItemNetInfo;
	if (nullptr != anItem)
	{
		ItemNetInfo = anItem->GetItemNetInfo();
	}

	TArray<ARPGPlayerControllerBase*> AllPlayerControllers = GetAllViewPlayerController();
	for (ARPGPlayerControllerBase* aPC : AllPlayerControllers)
	{
		if (aPC && !aPC->IsLocalPlayerController())
		{
			AddClientMessageQueue(aPC, ContainerType, NotifyType, ItemNetInfo, Value, bShowNotification);
		}
	}
}

void UGridContainerComponent::ClientMessageLoop()
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	if (ClientMessageQueue.IsEmpty())
	{
		return;
	}

	int32 Num = 0;
	while (Num <= MaxMessageValue)
	{
		Num++;

		FItemMsg* Message;
		if (!ClientMessageQueue.Dequeue(Message))
		{
			break;
		}

		if (Message->PlayerController.Get())
		{
			switch (Message->NotifyType)
			{
			case ENotifyItemType::NOTIFY_ITEM_ADD:
				Message->PlayerController.Get()->ClientAddItem(this, Message->ContainerType, Message->ItemNetInfo, Message->bShowNotification);
				break;
			case ENotifyItemType::NOTIFY_ITEM_REMOVE:
				Message->PlayerController.Get()->ClientRemoveItem(this, Message->ContainerType, Message->Value, Message->bShowNotification);
				break;
			case ENotifyItemType::NOTIFY_INVENTORY_CLEARALL:
				Message->PlayerController.Get()->ClientClearAllItem(this);
				break;
			case ENotifyItemType::NOTIFY_ITEM_SETAMOUNT:
				Message->PlayerController.Get()->ClientUpdateItem(this, Message->ContainerType, Message->ItemNetInfo, Message->bShowNotification);
				break;
			default:
				break;
			}
		}
		delete Message;
	}
}

bool UGridContainerComponent::AddItemByItemId(enum EContainerType ContainerType, int32 ItemId, EItemChangeReason ItemChangeReason, int32 AmountOverride /*= 1*/)
{
	if (!ContainerIndexMap.Find(ContainerType))
	{
		return false;
	}
	UBaseContainer* container = ContainerIndexMap[ContainerType];
	if (nullptr == container)
	{
		return false;
	}

	return container->AddItemByItemId(ItemId, ItemChangeReason, AmountOverride);
}

bool UGridContainerComponent::AddItemByNetInfo(enum EContainerType ContainerType, const FItemNetInfo& ItemNetInfo, bool bShowNotification /*= false*/)
{
	if (!ContainerIndexMap.Find(ContainerType))
	{
		return false;
	}

	UBaseContainer* container = ContainerIndexMap[ContainerType];
	if (nullptr == container)
	{
		return false;
	}

	return container->AddItemByNetInfo(ItemNetInfo, bShowNotification);
}

bool UGridContainerComponent::RemoveItemByUniqueId(enum EContainerType ContainerType, const FGuid& TheUniqueId, EItemChangeReason ItemChangeReason, int32 Amount /*= -1*/, bool bShowNotification /*= false */)
{
	if (!ContainerIndexMap.Find(ContainerType))
	{
		return false;
	}
	UBaseContainer* container = ContainerIndexMap[ContainerType];
	if (nullptr == container)
	{
		return false;
	}

	return container->RemoveItemByUniqueId(TheUniqueId, ItemChangeReason, Amount, bShowNotification);
}

bool UGridContainerComponent::RemoveItemBySlot(enum EContainerType ContainerType, int32 Slot, EItemChangeReason ItemChangeReason, int32 Amount /*= -1*/, bool bShowNotification /*= false */)
{
	if (!ContainerIndexMap.Find(ContainerType))
	{
		return false;
	}
	UBaseContainer* container = ContainerIndexMap[ContainerType];
	if (nullptr == container)
	{
		return false;
	}

	return container->RemoveItemBySlot(Slot, ItemChangeReason, Amount, bShowNotification);
}

bool UGridContainerComponent::UpdateItemBySlot(enum EContainerType ContainerType, int32 Slot, FItemNetInfo itemInfo, EItemChangeReason ItemChangeReason, bool bShowNotification /*= false*/)
{
	if (!ContainerIndexMap.Find(ContainerType))
	{
		return false;
	}
	UBaseContainer* container = ContainerIndexMap[ContainerType];
	if (nullptr == container)
	{
		return false;
	}

	return container->UpdateItemBySlot(Slot, itemInfo, ItemChangeReason, bShowNotification);
}

void UGridContainerComponent::ClearAllItem(enum EContainerType ContainerType /*= EContainerType::CONTAINER_TYPE_ALL*/)
{
	if (ContainerType == EContainerType::CONTAINER_TYPE_ALL)
	{
		for (auto& Elem : ContainerIndexMap)
		{
			UBaseContainer* container = Elem.Value;
			if (nullptr != container)
			{
				container->ClearAllItem();
			}
		}
	}
	else
	{
		if (!ContainerIndexMap.Find(ContainerType))
		{
			return;
		}
		UBaseContainer* container = ContainerIndexMap[ContainerType];
		if (nullptr == container)
		{
			return;
		}

		container->ClearAllItem();
	}
}

bool UGridContainerComponent::ExchangeItem(enum EContainerType SourceType, int32 SourceSlot, enum EContainerType TargetType, int32 TargetSlot, bool bShowNotification /*= false*/)
{
	if (!ContainerIndexMap.Find(SourceType) || !ContainerIndexMap.Find(TargetType))
	{
		return false;
	}

	UBaseContainer* SourceContainer = ContainerIndexMap[SourceType];
	if (nullptr == SourceContainer)
	{
		return false;
	}

	UBaseContainer* TargetContainer = ContainerIndexMap[TargetType];
	if (nullptr == TargetContainer)
	{
		return false;
	}

	return SourceContainer->ExchangeItem(SourceSlot, TargetContainer, TargetSlot, bShowNotification);
}

bool UGridContainerComponent::MoveItem(enum EContainerType SourceType, int32 SourceSlot, int32 MoveAmount, enum EContainerType TargetType, int32 TargetSlot, bool bShowNotification /*= false*/)
{
	if (!ContainerIndexMap.Find(SourceType) || !ContainerIndexMap.Find(TargetType))
	{
		return false;
	}

	UBaseContainer* SourceContainer = ContainerIndexMap[SourceType];
	if (nullptr == SourceContainer)
	{
		return false;
	}

	UBaseContainer* TargetContainer = ContainerIndexMap[TargetType];
	if (nullptr == TargetContainer)
	{
		return false;
	}

	SourceContainer->MoveItem(SourceSlot, MoveAmount, TargetContainer, TargetSlot, bShowNotification);

	return true;
}

void UGridContainerComponent::ServerGMAddItem_Implementation(int32 itemId, int32 AmountOverride /*= 1*/)
{

}


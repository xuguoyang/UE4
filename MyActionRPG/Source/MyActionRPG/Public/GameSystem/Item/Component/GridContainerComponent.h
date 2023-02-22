// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/RPGItemTypes.h"
#include "GridContainerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridComponentAddSignature, class UBaseContainer*, Container, EContainerType, ContainerType, int32, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridComponentRemoveSignature, class UBaseContainer*, Container, EContainerType, ContainerType, int32, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridComponentUpdateSignature, class UBaseContainer*, Container, EContainerType, ContainerType, int32, Slot);

struct FItemMsg
{
	TWeakObjectPtr<class ARPGPlayerControllerBase> PlayerController;
	enum EContainerType ContainerType;
	enum ENotifyItemType NotifyType;
	FItemNetInfo ItemNetInfo;
	int32 Value;
	bool bShowNotification;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UGridContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridContainerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void InitializeComponent() override;
public:
	ENetRole GetLocalRole();

	class APlayerCharacterBase* GetPlayer();

	class ARPGPlayerControllerBase* GetOwnerControlller();

	TArray<class ARPGPlayerControllerBase*> GetAllViewPlayerController();

	//初始化容器列表
	void InitContainerData();

	//同步初始化数据给客户端
	void ReplicateInitializeData();

	// 添加通知
	void AddClientMessageQueue(class ARPGPlayerControllerBase* PlayerController,
		enum EContainerType ContainerType,
		enum ENotifyItemType NotifyType,
		FItemNetInfo& ItemNetInfo,
		int32 Value,
		bool bShowNotification = false);

	// 通知客户端刷新状态
	void NotifyClientItemStatus(enum EContainerType ContainerType,
		enum ENotifyItemType NotifyType,
		class UBaseItem* anItem,
		int32 Value,
		bool bShowNotification = false);

	// 帧循环执行通知队列
	void ClientMessageLoop();

public:
	// 添加物品
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FGridComponentAddSignature OnAddItem;

	// 移除物品
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FGridComponentRemoveSignature OnRemoveItem;

	// 更新物品
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FGridComponentUpdateSignature OnUpdateItem;

public:
	// 通过ItemId创建道具
	UFUNCTION(BlueprintCallable)
	bool AddItemByItemId(
		enum EContainerType ContainerType,
		int32 ItemId,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	// 通过网络数据创建道具
	bool AddItemByNetInfo(
	     enum EContainerType ContainerType,
		 const FItemNetInfo& ItemNetInfo,
		 bool bShowNotification = false);

	//删除一格道具 返回删除是否成功, Amount = -1 表示移除全部道具
	bool RemoveItemByUniqueId(
		enum EContainerType ContainerType,
		const FGuid& TheUniqueId,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	// 删除一格道具 返回删除是否成功, Amount = -1 表示移除全部道具
	bool RemoveItemBySlot(
		enum EContainerType ContainerType,
		int32 Slot,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	//更新一格数据
	bool UpdateItemBySlot(enum EContainerType ContainerType,
		int32 Slot,
		FItemNetInfo itemInfo,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	//清除道具
	void ClearAllItem(enum EContainerType ContainerType = EContainerType::CONTAINER_TYPE_ALL);

	//交换物品
	bool ExchangeItem(enum EContainerType SourceType,
		int32 SourceSlot,
		enum EContainerType TargetType,
		int32 TargetSlot,
		bool bShowNotification = false);

	//移动物品
	bool MoveItem(enum EContainerType SourceType,
		int32 SourceSlot,
		int32 MoveAmount,
		enum EContainerType TargetType,
		int32 TargetSlot,
		bool bShowNotification = false);

public:
	//测试发道具
	UFUNCTION(Server, Reliable, Category = "Battle")
	void ServerGMAddItem(int32 itemId, int32 AmountOverride = 1);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EContainerType, TSubclassOf<class UBaseContainer>> ContainerMap;

	UPROPERTY(SaveGame)
	TMap<EContainerType, class UBaseContainer*> ContainerIndexMap;

	//每帧消息数量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
	int32 MaxMessageValue = 10;
private:
	UPROPERTY(Transient)
	TWeakObjectPtr<class APlayerCharacterBase> OwnerPlayerCharacter;

	UPROPERTY(Transient)
	TWeakObjectPtr<class ARPGPlayerControllerBase> OwnerPlayerController;

	// 通知队列
	TQueue<FItemMsg*> ClientMessageQueue;

	uint8 bLoadFromDataStore : 1;
};

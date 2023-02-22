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

	//��ʼ�������б�
	void InitContainerData();

	//ͬ����ʼ�����ݸ��ͻ���
	void ReplicateInitializeData();

	// ���֪ͨ
	void AddClientMessageQueue(class ARPGPlayerControllerBase* PlayerController,
		enum EContainerType ContainerType,
		enum ENotifyItemType NotifyType,
		FItemNetInfo& ItemNetInfo,
		int32 Value,
		bool bShowNotification = false);

	// ֪ͨ�ͻ���ˢ��״̬
	void NotifyClientItemStatus(enum EContainerType ContainerType,
		enum ENotifyItemType NotifyType,
		class UBaseItem* anItem,
		int32 Value,
		bool bShowNotification = false);

	// ֡ѭ��ִ��֪ͨ����
	void ClientMessageLoop();

public:
	// �����Ʒ
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FGridComponentAddSignature OnAddItem;

	// �Ƴ���Ʒ
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FGridComponentRemoveSignature OnRemoveItem;

	// ������Ʒ
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FGridComponentUpdateSignature OnUpdateItem;

public:
	// ͨ��ItemId��������
	UFUNCTION(BlueprintCallable)
	bool AddItemByItemId(
		enum EContainerType ContainerType,
		int32 ItemId,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	// ͨ���������ݴ�������
	bool AddItemByNetInfo(
	     enum EContainerType ContainerType,
		 const FItemNetInfo& ItemNetInfo,
		 bool bShowNotification = false);

	//ɾ��һ����� ����ɾ���Ƿ�ɹ�, Amount = -1 ��ʾ�Ƴ�ȫ������
	bool RemoveItemByUniqueId(
		enum EContainerType ContainerType,
		const FGuid& TheUniqueId,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	// ɾ��һ����� ����ɾ���Ƿ�ɹ�, Amount = -1 ��ʾ�Ƴ�ȫ������
	bool RemoveItemBySlot(
		enum EContainerType ContainerType,
		int32 Slot,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	//����һ������
	bool UpdateItemBySlot(enum EContainerType ContainerType,
		int32 Slot,
		FItemNetInfo itemInfo,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	//�������
	void ClearAllItem(enum EContainerType ContainerType = EContainerType::CONTAINER_TYPE_ALL);

	//������Ʒ
	bool ExchangeItem(enum EContainerType SourceType,
		int32 SourceSlot,
		enum EContainerType TargetType,
		int32 TargetSlot,
		bool bShowNotification = false);

	//�ƶ���Ʒ
	bool MoveItem(enum EContainerType SourceType,
		int32 SourceSlot,
		int32 MoveAmount,
		enum EContainerType TargetType,
		int32 TargetSlot,
		bool bShowNotification = false);

public:
	//���Է�����
	UFUNCTION(Server, Reliable, Category = "Battle")
	void ServerGMAddItem(int32 itemId, int32 AmountOverride = 1);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EContainerType, TSubclassOf<class UBaseContainer>> ContainerMap;

	UPROPERTY(SaveGame)
	TMap<EContainerType, class UBaseContainer*> ContainerIndexMap;

	//ÿ֡��Ϣ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
	int32 MaxMessageValue = 10;
private:
	UPROPERTY(Transient)
	TWeakObjectPtr<class APlayerCharacterBase> OwnerPlayerCharacter;

	UPROPERTY(Transient)
	TWeakObjectPtr<class ARPGPlayerControllerBase> OwnerPlayerController;

	// ֪ͨ����
	TQueue<FItemMsg*> ClientMessageQueue;

	uint8 bLoadFromDataStore : 1;
};

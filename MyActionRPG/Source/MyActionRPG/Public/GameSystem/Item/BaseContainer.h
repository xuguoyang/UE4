// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define/RPGItemTypes.h"
#include "BaseContainer.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API UBaseContainer : public UObject
{
	GENERATED_BODY()
	
public:
	//��ʼ������
	void InitContainer(class UGridContainerComponent* grid);

	void InitItemOwner();

private:
	AActor* GetOwner();

	//��������
#pragma region  Operation
public:
	// ͨ��ItemSubclass�������ߣ�������������
	class UBaseItem* CreateItemByClass(
		TSubclassOf<class UBaseItem> ItemSubclass,
		int32 AmountOverride = 1);

	// �½��������ߣ��½����߲��Ҽ��뱳��,���AmountOverride>MaxAmountʱ����ݹ�����Լ�������ֵ��ȷ����
	bool AddItemByClass(
		TSubclassOf<class UBaseItem> ItemSubclass,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	// �½��������ߣ��½����߲��Ҽ��뱳��, ��ͼʹ��)
	UFUNCTION(BlueprintCallable)
	bool AddItemByClassSimple(
		TSubclassOf<class UBaseItem> ItemSubclass,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	// ͨ��ItemId�������ߣ�������������
	UFUNCTION(BlueprintCallable)
	bool AddItemByItemId(
		int32 ItemId,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	//ͨ���������ݴ�������
	bool AddItemByNetInfo(const FItemNetInfo& ItemNetInfo,
		bool bShowNotification = false);

	// Slot��ȷ��ӱ�������
	bool AddItemToExactSlot(class UBaseItem* newItem, bool bShowNotification = false);

	// Slot����ȷ��ӱ�������  Ŀǰֻ�ѵ���һ��Ѱ�ҵ��Ŀ��Զѵ�����Ʒ�����Զѵ�����Ʒ��Ѱ�Ҷѵ�������
	bool AddItemToInExactSlot(class UBaseItem* newItem, bool bShowNotification = false);

	// ��Ӷ�Ӧ��Ʒ�������� ֱ�����Ӳ�Ҫʹ�ô˺���
	bool AddItemToSlot(class UBaseItem* newItem,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	bool TestAddItemToSlot(class UBaseItem* newItem, int32 SlotIndex);

	// �ѵ���Ʒ ����ʣ����Ҫ�ѵ����� 0 Ϊ�ѵ���� 
	int32 MergeItem(
	class UBaseItem* NeedMergeItem,
		int32 MergeAmount,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	// Ѱ�ҵ�һ���ɶѵ����� �����е�����ͬ���Ҵ˵�������С�����ѵ����� ��ɶѵ�
	class UBaseItem* GetCanMergeItem(class UBaseItem* newItem, int32& StartIndex);

	//����Ƿ��п���λ��
	bool CheckEmptySlot(class UBaseItem* newItem);

	// Ѱ�ұ����е�һ��������λ  ����-1��û�ҵ�
	int32 GetFirstEmptySlot();

	//ɾ��һ����� ����ɾ���Ƿ�ɹ�, Amount = -1 ��ʾ�Ƴ�ȫ������
	bool RemoveItemByUniqueId(
		const FGuid& TheUniqueId,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	// ɾ��һ����� ����ɾ���Ƿ�ɹ�, Amount = -1 ��ʾ�Ƴ�ȫ������
	bool RemoveItemBySlot(
		int32 Slot,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	bool TestRemoveItemBySlot(int32 Slot);

	//����һ������
	bool UpdateItemBySlot(
		int32 Slot,
		FItemNetInfo itemInfo,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	//�������
	void ClearAllItem();

	//������Ʒ��Ϣ�¼�
	void NoticeUpdateItem(class UBaseItem* anItem,
		enum ENotifyItemType NotifyType,
		bool bShowNotification = false);

	//�����Ƿ���Ч
	bool IsValidIndex(int32 Slot);

	//���ݸ���������ȡ��Ʒ
	class UBaseItem* FindItemBySlot(int32 Slot);

	//������Ʒ
	bool ExchangeItem(int32 SourceSlot,
		class UBaseContainer* TargetContainer,
		int32 TargetSlot,
		bool bShowNotification = false);

	//�ƶ���Ʒ
	int32 MoveItem(int32 SourceSlot,
		int32 MoveAmount,
		class UBaseContainer* TargetContainer,
		int32 TargetSlot,
		bool bShowNotification = false);

private:
	bool CheckMove(int32 SourceSlot,
		class UBaseContainer* TargetContainer,
		int32 TargetSlot);


#pragma endregion

public:
	//��Ʒ�б�
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TArray<class UBaseItem*> Items;

	// ������������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame)
	int32 ItemMaxAmount;

	//��ҳ��С
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ShowOnePageAmount = 10;

	//������ޱ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAddGroup = 10;

	//��������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EContainerType ContainerType;

	//�����ֿ����
	TWeakObjectPtr<class UGridContainerComponent> GridComponent;
};

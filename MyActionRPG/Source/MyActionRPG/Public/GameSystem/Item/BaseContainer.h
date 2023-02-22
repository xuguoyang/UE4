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
	//初始化容器
	void InitContainer(class UGridContainerComponent* grid);

	void InitItemOwner();

private:
	AActor* GetOwner();

	//容器操作
#pragma region  Operation
public:
	// 通过ItemSubclass创建道具，返回新增道具
	class UBaseItem* CreateItemByClass(
		TSubclassOf<class UBaseItem> ItemSubclass,
		int32 AmountOverride = 1);

	// 新建背包道具（新建道具并且加入背包,如果AmountOverride>MaxAmount时，会递归调用自己。返回值不确定）
	bool AddItemByClass(
		TSubclassOf<class UBaseItem> ItemSubclass,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	// 新建背包道具（新建道具并且加入背包, 蓝图使用)
	UFUNCTION(BlueprintCallable)
	bool AddItemByClassSimple(
		TSubclassOf<class UBaseItem> ItemSubclass,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	// 通过ItemId创建道具，返回新增道具
	UFUNCTION(BlueprintCallable)
	bool AddItemByItemId(
		int32 ItemId,
		EItemChangeReason ItemChangeReason,
		int32 AmountOverride = 1);

	//通过网络数据创建道具
	bool AddItemByNetInfo(const FItemNetInfo& ItemNetInfo,
		bool bShowNotification = false);

	// Slot明确添加背包道具
	bool AddItemToExactSlot(class UBaseItem* newItem, bool bShowNotification = false);

	// Slot不明确添加背包道具  目前只堆叠第一个寻找到的可以堆叠的物品（可以堆叠的物品见寻找堆叠函数）
	bool AddItemToInExactSlot(class UBaseItem* newItem, bool bShowNotification = false);

	// 添加对应物品到背包中 直接增加不要使用此函数
	bool AddItemToSlot(class UBaseItem* newItem,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	bool TestAddItemToSlot(class UBaseItem* newItem, int32 SlotIndex);

	// 堆叠物品 返回剩余需要堆叠数量 0 为堆叠完成 
	int32 MergeItem(
	class UBaseItem* NeedMergeItem,
		int32 MergeAmount,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	// 寻找第一个可堆叠道具 背包中道具相同并且此道具数量小于最大堆叠数量 则可堆叠
	class UBaseItem* GetCanMergeItem(class UBaseItem* newItem, int32& StartIndex);

	//检测是否有空余位置
	bool CheckEmptySlot(class UBaseItem* newItem);

	// 寻找背包中第一个空余栏位  返回-1则没找到
	int32 GetFirstEmptySlot();

	//删除一格道具 返回删除是否成功, Amount = -1 表示移除全部道具
	bool RemoveItemByUniqueId(
		const FGuid& TheUniqueId,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	// 删除一格道具 返回删除是否成功, Amount = -1 表示移除全部道具
	bool RemoveItemBySlot(
		int32 Slot,
		EItemChangeReason ItemChangeReason,
		int32 Amount = -1,
		bool bShowNotification = false
	);

	bool TestRemoveItemBySlot(int32 Slot);

	//更新一格数据
	bool UpdateItemBySlot(
		int32 Slot,
		FItemNetInfo itemInfo,
		EItemChangeReason ItemChangeReason,
		bool bShowNotification = false);

	//清除背包
	void ClearAllItem();

	//更新物品信息事件
	void NoticeUpdateItem(class UBaseItem* anItem,
		enum ENotifyItemType NotifyType,
		bool bShowNotification = false);

	//索引是否有效
	bool IsValidIndex(int32 Slot);

	//根据格子索引获取物品
	class UBaseItem* FindItemBySlot(int32 Slot);

	//交换物品
	bool ExchangeItem(int32 SourceSlot,
		class UBaseContainer* TargetContainer,
		int32 TargetSlot,
		bool bShowNotification = false);

	//移动物品
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
	//物品列表
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TArray<class UBaseItem*> Items;

	// 格子上限数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame)
	int32 ItemMaxAmount;

	//分页大小
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ShowOnePageAmount = 10;

	//最大上限倍数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAddGroup = 10;

	//容器类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EContainerType ContainerType;

	//所属仓库组件
	TWeakObjectPtr<class UGridContainerComponent> GridComponent;
};

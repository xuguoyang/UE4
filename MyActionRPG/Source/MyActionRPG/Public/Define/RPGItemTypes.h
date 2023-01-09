// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include <Engine/EngineTypes.h>
#include "RPGItemTypes.generated.h"

// 道具类型
UENUM(BlueprintType)
enum class EItemType : uint8
{
    ItemType_Equip,
    ItemType_Task,
};

// 装备类型
UENUM(BlueprintType)
enum class EItemEquipType : uint8
{
    ItemEquipType_None,
    ItemEquipType_Head,         // 头部装备
    ItemEquipType_Body,         // 衣服装备
    ItemEquipType_Hand,         // 手部装备
    ItemEquipType_Weapon,       // 武器装备
    ItemEquipType_Leg,          // 腿部装备
    ItemEquipType_Foot,         // 脚步装备
};

// 快捷栏类型
UENUM(BlueprintType)
enum class EShortcutType: uint8
{
    ShortcutGirdType_None,
    ShortcutGirdType_Item,
    ShortcutGirdType_Skill,
    ShortcutGirdType_WeaponSkill
};

USTRUCT(Blueprintable)
struct FItemNetInfo
{
	GENERATED_USTRUCT_BODY()

	// 唯一ID 全
	UPROPERTY(VisibleAnywhere)
	FGuid UniqueId;

	// 道具在表格中的Index 全
	UPROPERTY(VisibleAnywhere)
	int32 ItemIndex;

	// 在第几个格子中 全
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SlotIndex;

	// 道具的数量全
	UPROPERTY(VisibleAnywhere)
	int32 Amount;

	// 道具的信息 全
	UPROPERTY(VisibleAnywhere)
	FString JsonStr;

	FItemNetInfo()
	{
		UniqueId = FGuid();
		ItemIndex = -1;
		SlotIndex = -1;
		Amount = -1;
		JsonStr = TEXT("");
	}
};

// 背包新建道具原因
// 添加只能在最后添加
UENUM()
enum class EItemChangeReason : uint8
{
	InvalidReason,
	// 商店
	Shop,
	// 任务
	Quest,
	// GM
	GM,
	// 邮件附件
	MailAttachment,
	// 掉落道具
	DropItem,
	// 拆分
	SplitItem,
	// 合并
	MergeItem,
	// 使用
	Use,
	// 清空
	ClearAll,
	//交换
	Exchange,
};

UENUM(BlueprintType)
enum class EContainerType : uint8
{
	CONTAINER_TYPE_BAG UMETA(DisplayName = "Bag"),
	CONTAINER_TYPE_EQUIPMENT UMETA(DisplayName = "Equipment"),
	CONTAINER_TYPE_SHORTCUT UMETA(DisplayName = "ShortCut"),
	CONTAINER_TYPE_APPEARANCE UMETA(DisplayName = "Appearance"),
	CONTAINER_TYPE_ALL
};

UENUM(BlueprintType)
enum class ENotifyItemType : uint8
{
	NOTIFY_NONE,
	NOTIFY_ITEM_UPDATENETINFO,
	NOTIFY_ITEM_ADD,
	NOTIFY_ITEM_REMOVE,
	NOTIFY_ITEM_EXCHANGE,
	NOTIFY_ITEM_SETAMOUNT,
	NOTIFY_INVENTORY_CLEARALL,
};

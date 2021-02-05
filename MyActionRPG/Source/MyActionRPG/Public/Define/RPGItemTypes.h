// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include "Engine/EngineTypes.h"
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


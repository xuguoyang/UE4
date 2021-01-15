// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include "Engine/EngineTypes.h"
#include "RPGItemTypes.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    ItemType_Equip,
    ItemType_Task,
};

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

USTRUCT(BlueprintType)
struct FItemGird
{
    GENERATED_USTRUCT_BODY()

    FItemGird() :Item(nullptr) {}

    UPROPERTY()
    uint32 Index;                   // 索引

    UPROPERTY()
    class UItemBase* Item;          // 当前道具

    bool IsEmpty()
    {
        return Item == nullptr;
    }

    void Reset()
    {
        Item = nullptr;
    }
};

USTRUCT(BlueprintType)
struct FEquipGird
{
    GENERATED_USTRUCT_BODY()

    FEquipGird() :Item(nullptr) {}

    UPROPERTY(EditAnywhere)
    EItemEquipType Type;                   // 当前位置装备类型

    UPROPERTY()
    class UItemBase* Item;                 // 当前道具

    bool IsEmpty()
    {
        return Item == nullptr;
    }

    void Reset()
    {
        Item = nullptr;
    }
};


#pragma once

#include "MyActionRPG.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "SlateBrush.h"
#include "Define/RPGItemTypes.h"
#include "ItemActorBase.h"
#include "RPGItemDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FRPGItemDataTableRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FRPGItemDataTableRow() {}

    /** 道具ID*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    FName ItemID;

    /** 道具类型*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    EItemType ItemType;

    /** 装备道具类型*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    EItemEquipType ItemEquipType;

    /** 装备道具要求的属性（角色等级,力量，敏捷，智力）*/


    /** 道具Actor*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    TSubclassOf<AItemActorBase> ItemActor;
};
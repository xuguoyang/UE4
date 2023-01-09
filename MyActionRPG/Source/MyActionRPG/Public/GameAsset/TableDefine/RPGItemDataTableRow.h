#pragma once
#include <Engine/DataTable.h>
#include <GameFramework/Actor.h>
#include <Styling/SlateBrush.h>
#include "Define/RPGItemTypes.h"
#include "GameSystem/Item/ItemActorBase.h"
#include "GameSystem/Item/ItemBase.h"
#include "RPGItemDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FRPGItemDataTableRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FRPGItemDataTableRow() 
    {
        ItemObjClass = UItemBase::StaticClass();
        CanStacked = true;
    }

    /** 道具ID*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    FName ItemID;

    /** 道具类型*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    EItemType ItemType;

    /** 装备道具类型*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    EItemEquipType ItemEquipType;

    /** 是否可堆叠*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    bool CanStacked;

    /** 道具UObject*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    TSubclassOf<UItemBase> ItemObjClass;

    /** 道具Actor*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    TSubclassOf<AItemActorBase> ItemActorClass;

    /** 道具Actor的绑点*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    FName BindSocketName;

    /** 道具附带的技能*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
    TSubclassOf<URPGGameplayAbility> AbilityClass;

	/**  用来显示的icon路径 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ITEM)
	FSoftObjectPath IconPath;
};
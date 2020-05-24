// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyActionRPG.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "RPGAssetManager.h"
#include "RPGItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class MYACTIONRPG_API URPGItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URPGItem()
	{}

    /** 道具类型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType	ItemType;

    /** 道具名 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
    FText ItemName;

    /** 道具描述 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
    FText ItemDescription;

    /** 用来显示的icon  */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
    FSlateBrush ItemIcon;

    /** 可以一次被装备的最大数量*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
    int32 MaxCount;

    /** 当前道具是否是消耗品*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
    bool IsConsumble() const;

    /** 道具的最高等级， <=0意味着无上限*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
    int32 MaxLevel;

    /** 当前道具绑定的技能*/
    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
    //TSubclassOf<U>
    /** 技能等级，<=0意味着角色等级*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
    int32 AbilityLevel;

    /** 获取道具主ID*/
    UFUNCTION(BlueprintCallable, Category = Item)
    FString GetIdentifierString() const;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};

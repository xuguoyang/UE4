// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGItemDataTableRow.h"
#include "WeakObjectPtrTemplates.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MYACTIONRPG_API UItemBase : public UObject
{
	GENERATED_BODY()
public:
    UItemBase();
protected:
    virtual bool IsSupportedForNetworking() const;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const;
public:
    /** 道具ID*/
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemID, Category = ITEM)
    FName ItemID;

    /** 道具等级*/
    UPROPERTY(BlueprintReadWrite, Replicated, Category = ITEM)
    int32 ItemLevel;
    
    /** 当前道具的配置*/
    struct FRPGItemDataTableRow* ItemConfig;
    
private:
    UFUNCTION()
    void OnRep_ItemID();

};

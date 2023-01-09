// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
class UItemContainerComponent;
UCLASS(Blueprintable, BlueprintType)
class MYACTIONRPG_API UItemBase : public UObject
{ 
	GENERATED_BODY()
public:
    UItemBase();

    virtual void OnAdd(UItemContainerComponent* ContainerComponent);
    virtual void OnRemove();
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

    /** 道具数量*/
    UPROPERTY(BlueprintReadWrite, Replicated, Category = ITEM)
    int32 ItemCount;

    /** 当前道具的配置*/
    struct FRPGItemDataTableRow* ItemConfig;
    
    /** 当前道具的Actor*/
    UPROPERTY(BlueprintReadOnly)
    AActor* ItemActor;

    /** 当前道具的Owner*/
    UPROPERTY(BlueprintReadOnly)
    AActor* OwnerActor;

    /** 当前道具所在容器组件*/
    UPROPERTY(BlueprintReadOnly)
    class UItemContainerComponent* OwnerContainerComponent;
private:
    UFUNCTION()
    void OnRep_ItemID();

};

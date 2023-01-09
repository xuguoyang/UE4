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
    /** ����ID*/
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemID, Category = ITEM)
    FName ItemID;

    /** ���ߵȼ�*/
    UPROPERTY(BlueprintReadWrite, Replicated, Category = ITEM)
    int32 ItemLevel;

    /** ��������*/
    UPROPERTY(BlueprintReadWrite, Replicated, Category = ITEM)
    int32 ItemCount;

    /** ��ǰ���ߵ�����*/
    struct FRPGItemDataTableRow* ItemConfig;
    
    /** ��ǰ���ߵ�Actor*/
    UPROPERTY(BlueprintReadOnly)
    AActor* ItemActor;

    /** ��ǰ���ߵ�Owner*/
    UPROPERTY(BlueprintReadOnly)
    AActor* OwnerActor;

    /** ��ǰ���������������*/
    UPROPERTY(BlueprintReadOnly)
    class UItemContainerComponent* OwnerContainerComponent;
private:
    UFUNCTION()
    void OnRep_ItemID();

};

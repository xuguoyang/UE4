// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGItemTypes.h"
#include "ItemEquipComponent.generated.h"

class UItemBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UItemEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemEquipComponent();

public:
    /** 装备道具 */
    UFUNCTION(BlueprintCallable, Category = Equip)
    bool EquipItem(UItemBase* Item);

    /** 卸载道具*/
    UFUNCTION(BlueprintCallable, Category = Equip)
    bool RemoveItem(int32 Index);

    UFUNCTION(BlueprintCallable, Category = Equip)
    bool CanEquipItem(UItemBase* Item);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
    /** 装备格子的类型在蓝图中配置*/
    UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_EquipArrayChange, Category = Equip)
    TArray<FEquipGird> EquipArray;

private:
    UFUNCTION()
    void OnRep_EquipArrayChange();

private:
    /**获取某个类型所有的装备栏序号*/
    typedef TArray<uint8> OutGirdArray;
    bool GetEquipGird(EItemEquipType Type, OutGirdArray& OutArrary);
};

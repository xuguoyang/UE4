// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGItemTypes.h"
#include "ShortcutComponent.generated.h"

// 快捷栏格子
USTRUCT(BlueprintType)
struct FShortcutGird
{
    GENERATED_USTRUCT_BODY()

        FShortcutGird() :
        Index(-1),
        Item(nullptr)
    {}

    // 格子索引
    UPROPERTY()
    uint32 Index;

    // 添加的对象
    UPROPERTY()
    UObject* Item;

    // 快捷栏的类型
    UPROPERTY(EditDefaultsOnly, Category = Shortcut)
    EShortcutType Type;

    // 使用物品
    bool UseItem();

    // 添加道具
    void Add(UObject* InItem);

    // 清空
    void Clear();

    // 是否可以添加
    bool CanAdd(UObject* InItem);

    /** 可以支持配置一个默认的技能*/
    UPROPERTY(EditDefaultsOnly, Category = Shortcut)
    TSubclassOf<UGameplayAbility> Default;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UShortcutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShortcutComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
    /** 添加到快捷栏*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    bool AddShortcutItem(UObject* Item, int32 Index = -1);

    /** 从快捷栏中移除*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    bool RemoveShortcutItem(int32 Index);

    /** 使用某个快捷栏中的对象*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    bool ShortcutUse(int32 Index);

    /** 获得某个对象的类型*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    EShortcutType GetShortcutType(UObject* Item);

// 向客户端发消息
protected:
    UFUNCTION(Reliable, Client, BlueprintCallable)
    void ClientAddSuccessed(int32 Index);

    UFUNCTION(Reliable, Client, BlueprintCallable)
    void ClientRemoveSuccessed(int32 Index);

// 向服务端发消息
protected:
    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
    void ServerAddShortcutItem(int32 Index, UObject* Item);

    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
    void ServerRemoveShortcutItem(int32 Index);

    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
    void ServerShortcutUse(int32 Index);

private:
    /** 索引是否有效*/
    bool IsIndexVaild(int32 Index);

    /** 是否可以添加到快捷栏*/
    bool CanAdd(int32 Index, UObject* Item);

    /** 是否可以移除到快捷栏*/
    bool CanRemove(int32 Index);

    /** 是否可以使用*/ 
    bool CanUse(int32 Index);

    /** 获取一个空的格子*/
    int32 GetEmptyGird(EShortcutType Type);

    /** 根据配置初始化添加技能*/
    void InitAbilityAdd();
protected:
    // 所有快捷栏格子,在蓝图中配置
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Default)
    TArray<FShortcutGird> ShortcutGirds;
};

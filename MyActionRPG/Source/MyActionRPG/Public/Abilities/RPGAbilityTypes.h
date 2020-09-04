// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include "RPGTargetType.h"
#include "GameplayAbilityTargetTypes.h"
#include "Engine/EngineTypes.h"
#include "RPGAbilityTypes.generated.h"

/** 
*   gameplay effect class列表、a tag, and targeting info
*  这个容器在蓝图或assets中被静态定义(指定类型和默认值)然后在运行时被转换成Specs再执行
*/
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainer
{
    GENERATED_BODY()

public:
    FRPGGameplayEffectContainer() {}

    /**设定技能目标类型*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TSubclassOf<URPGTargetType> TargetType;

    /** 应用于当前目标的技能效果类型 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
   
};

/** 对应FRPGGameplayEffectContainer真正用于执行计算的版本 */
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainerSpec
{
    GENERATED_BODY()
public:
    FRPGGameplayEffectContainerSpec() {}

    /** 技能系统内部目标数据*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    FGameplayAbilityTargetDataHandle TagetData;

    /** 应用于当前目标的技能效果对象列表 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

    /** 是否含有有效的effect specs*/
    bool HasValidEffects() const;

    /** 是否含有有效的targets */
    bool HasValidTargets() const;

    /** 新增目标数据*/
    void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};
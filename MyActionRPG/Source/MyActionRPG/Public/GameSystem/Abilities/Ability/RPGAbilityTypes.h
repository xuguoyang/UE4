// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include "RPGTargetType.h"
#include "GameplayAbilityTargetTypes.h"
#include "Engine/EngineTypes.h"
#include "RPGAbilityTypes.generated.h"


/** 扩展FGameplayAbilitySpc，所有动态的属性加在这里*/
USTRUCT(BlueprintType)
struct FRPGGameplayAbilitySpc : public FGameplayAbilitySpec
{
    GENERATED_USTRUCT_BODY()
    
    FRPGGameplayAbilitySpc() :
    FGameplayAbilitySpec() {}

    FRPGGameplayAbilitySpc(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject* InSourceObject = nullptr):
    FGameplayAbilitySpec(InAbilityClass, InLevel, InInputID, InSourceObject)
    {}

    /** */
};

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
    FGameplayAbilityTargetDataHandle TargetData;

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

/** 攻击距离枚举 */
UENUM(BlueprintType)
enum class EAttackDisType: uint8
{
    AttackDisType_None,
    AttackDisType_Near,             // 近战
    AttackDisType_Far               // 远程
};

/** 释放方式 */
UENUM(BlueprintType)
enum class EAbilityApplyType:uint8
{
    AbilityApplyType_Actions,       // 主动释放
    AbilityApplyType_Passive        // 被动技能
};

/** 技能的静态配置数据 **/
USTRUCT(BlueprintType)
struct FGameplayAbilityStaticData
{
    GENERATED_BODY()

    /** 技能图标*/
    UPROPERTY(EditDefaultsOnly)
    FSoftObjectPath   Icon;

    /** 释放方式（主动，被动）*/
    UPROPERTY(EditDefaultsOnly)
    EAttackDisType AttackDisType = EAttackDisType::AttackDisType_Near;

    /** 释放距离（近战、远程）*/
    UPROPERTY(EditDefaultsOnly)
    EAbilityApplyType AbilityApplyType = EAbilityApplyType::AbilityApplyType_Actions;

};
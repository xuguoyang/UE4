// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyActionRPG.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "RPGAbilityTypes.h"
#include "GameplayAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "RPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
    URPGGameplayAbility();

    /** GameplayTag对应的EffectContainer容器, 在蓝图中配置*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
    TMap<FGameplayTag, FRPGGameplayEffectContainer> EffectContainerMap;

    /** 通过创建spec执行某一个技能容器 */
    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

    /** 通过Gameplaytag搜索EffectContainerMap获取EffectContainer生成EffectContainerSpec*/
    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual FRPGGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

    /** 通过EffectContainer生成EffectContainerSpec*/
    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual FRPGGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FRPGGameplayEffectContainer& Contanier, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

    /** 执行之前已经创建好的ContainerSpec, 在这里是真正执行技能效果*/
    UFUNCTION(BlueprintCallable, Category = Ability)
    virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyActionRPG.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilityTypes.h"
#include "RPGTargetType.generated.h"

/**
 * 用于技能释放目标的筛选策略
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class MYACTIONRPG_API URPGTargetType : public UObject
{
	GENERATED_BODY()
public:
    /** 获取技能效果应用的目标, 蓝图可以覆盖此函数 */
    UFUNCTION(BlueprintNativeEvent)
    void GetTargets(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};


/** 给自身施加的技能*/
UCLASS(NotBlueprintable)
class MYACTIONRPG_API URPGTargetType_UseOwner:public URPGTargetType
{
    GENERATED_BODY()
public:
    virtual void GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** 通过技能事件获取目标 */
UCLASS(NotBlueprintable)
class MYACTIONRPG_API URPGTargetType_UseEventData :public URPGTargetType
{
    GENERATED_BODY()
public:
    virtual void GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

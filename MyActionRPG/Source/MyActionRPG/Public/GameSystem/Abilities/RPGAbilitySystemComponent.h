// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FAbilityClassData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<URPGGameplayAbility> AbilityClass;
};
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYACTIONRPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()  
public:
    URPGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);
public:
    static URPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

    void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URPGGameplayAbility*>& ActiveAbilities);

    /**刷新所有属性 */
    void RefreshAllAttribute();

    /**设置技能等级*/
    bool SetAbilityLevelByClass(TSubclassOf<URPGGameplayAbility>& AbilityClass, int32 TargetLevel);

    /**根据等级更新技能和属性*/
    bool UpdateAbilityAndAttributeByLevel(int32 TargetLevel);

    /** 执行GamePlayCue， 效果会同步到所有关联的客户端*/
    UFUNCTION(BlueprintCallable, Category = GameplayCue)
    void ExecuteGamePlayCueInSever(const FGameplayTag GameplayCueTag);

    /** 在客户端本地执行GamePlayCue*/
    UFUNCTION(BlueprintCallable, Category = GameplayCue)
    void ExecuteGamePlayCueInLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameter);

// 技能配置
public:
    /** 基础属性效果*/
    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameplayEffect> BasicAttributeEffectClass;

    /** 技能配置*/
    UPROPERTY(EditAnywhere)
    TArray<FAbilityClassData> AbilityClasses;

    UPROPERTY()
    int32 bAbilitiesInitialized;

private:
    virtual void BeginPlay() override;

private:
    /** 初始化角色属性*/
    void InitAttribute();

    /** 初始化技能*/
    void InitAbility();

    /** 添加基础属性*/
    void AddBaseAttributeGE();

    /** 移除基础属性*/
    void RemoveBaseAttributeGE();

    /** 添加基础技能*/
    void AddBaseAbility();

    /** 移除基础技能*/
    void RemoveBaseAbility();
// 组件对外的通知
private:
    void NotifyAbilityInitialized();

private:
    TArray<FGameplayAbilitySpecHandle> ActiveBaseGAs;
    TArray<FActiveGameplayEffectHandle> ActiveBaseGEs;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "RPGAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "Interface/ShortcutUseInterface.h"
#include "RPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGGameplayAbility : public UGameplayAbility, public IShortcutUseInterface
{
	GENERATED_BODY()
public:
    URPGGameplayAbility();

    /** GameplayTag��Ӧ��EffectContainer����, ����ͼ������*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
    TMap<FGameplayTag, FRPGGameplayEffectContainer> EffectContainerMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
    FGameplayAbilityStaticData  GAStaticData;
public:
    /** ͨ������specִ��ĳһ���������� */
    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

    /** ͨ��Gameplaytag����EffectContainerMap��ȡEffectContainer����EffectContainerSpec*/
    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual FRPGGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

    /** ͨ��EffectContainer����EffectContainerSpec*/
    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual FRPGGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FRPGGameplayEffectContainer& Contanier, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

    /** ִ��֮ǰ�Ѿ������õ�ContainerSpec, ������������ִ�м���Ч��*/
    UFUNCTION(BlueprintCallable, Category = Ability)
    virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec);

public:
    virtual bool ShortcutUse() override;
    virtual bool OnAddShortcut() override;
    virtual bool OnRemoveShortcut() override;
    virtual EShortcutType GetType() override;

protected:
    UFUNCTION(BlueprintNativeEvent)
    void UseAbility();

    UFUNCTION(BlueprintPure)
    bool AbilityIsActive();
};

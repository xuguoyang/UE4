// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameplayAbility.h"
#include "RPGCharacterBase.h"
#include "Abilities/RPGAbilitySystemComponent.h"

URPGGameplayAbility::URPGGameplayAbility()
{

}

TArray<FActiveGameplayEffectHandle> URPGGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel /*= -1*/)
{
    FRPGGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
    return ApplyEffectContainerSpec(Spec);
}

FRPGGameplayEffectContainerSpec URPGGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel /*= -1*/)
{
    FRPGGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);
    if (FoundContainer)
    {
        return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
    }

    return FRPGGameplayEffectContainerSpec();
}

FRPGGameplayEffectContainerSpec URPGGameplayAbility::MakeEffectContainerSpecFromContainer(const FRPGGameplayEffectContainer& Contanier, const FGameplayEventData& EventData, int32 OverrideGameplayLevel /*= -1*/)
{
    FRPGGameplayEffectContainerSpec ReturnSpec;
    // 获取技能系统组件对象
    AActor* OwningActor = GetOwningActorFromActorInfo();
    ARPGCharacterBase* OwningCharacter = Cast<ARPGCharacterBase>(OwningActor);
    URPGAbilitySystemComponent* OwningASC = URPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);
    if (OwningASC)
    {
        // 如果我们有一个目标类型，执行命中逻辑，实际目标可以在后面再添加
        if (Contanier.TargetType.Get())
        {
            TArray<FHitResult> HitResults;          // 逐个碰撞到获取到的目标
            TArray<AActor*> TargetActors;           // 范围技能获取到的目标，或自身

            // 通过配置的TargetType子类型的CDO的AddTargets函数获取HitResults和TargetActors
            // 通过传入的EventData数据确定目标Actor
            const URPGTargetType* TargetTypeCDO = Contanier.TargetType.GetDefaultObject();
            AActor* AvatarActor = GetAvatarActorFromActorInfo();
            TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
            ReturnSpec.AddTargets(HitResults, TargetActors);
        }
    }
    return ReturnSpec;
}

TArray<FActiveGameplayEffectHandle> URPGGameplayAbility::ApplyEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec)
{
    TArray<FActiveGameplayEffectHandle> AllActiveEffects;
    for (const FGameplayEffectSpecHandle& SpecHandle: ContainerSpec.TargetGameplayEffectSpecs)
    {
        AllActiveEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TagetData));
    }

    return AllActiveEffects;
}

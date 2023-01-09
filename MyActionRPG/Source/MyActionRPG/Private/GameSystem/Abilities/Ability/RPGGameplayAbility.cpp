// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Abilities/Ability/RPGGameplayAbility.h"
#include "GameSystem/Roles/RPGCharacterBase.h"
#include "GameSystem/Abilities/RPGAbilitySystemComponent.h"

URPGGameplayAbility::URPGGameplayAbility()
{
    
}

TArray<FActiveGameplayEffectHandle> URPGGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel /*= -1*/)
{
    // 只允许在服务端执行
    if (!GetAvatarActorFromActorInfo()->HasAuthority())
    {
        return TArray<FActiveGameplayEffectHandle>();
    }

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

        if (OverrideGameplayLevel == INDEX_NONE)
        {
            OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
        }

        // 创建GameplayEffectSpecs
        for (const TSubclassOf<UGameplayEffect>& EffectClass : Contanier.TargetGameplayEffectClasses)
        {
            ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
        }
    }
    return ReturnSpec;
}

TArray<FActiveGameplayEffectHandle> URPGGameplayAbility::ApplyEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec)
{
    TArray<FActiveGameplayEffectHandle> AllActiveEffects;
    for (const FGameplayEffectSpecHandle& SpecHandle: ContainerSpec.TargetGameplayEffectSpecs)
    {
        AllActiveEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
    }

    return AllActiveEffects;
}

bool URPGGameplayAbility::ShortcutUse()
{
   /* UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    if (AbilitySystemComponent)
    {
        return AbilitySystemComponent->TryActivateAbilityByClass(this->GetClass());
    }*/
    UseAbility();
    return true;
}

bool URPGGameplayAbility::OnAddShortcut()
{
    return true;
}

bool URPGGameplayAbility::OnRemoveShortcut()
{
    return true;
}

EShortcutType URPGGameplayAbility::GetType()
{
    return EShortcutType::ShortcutGirdType_Skill;
}

void URPGGameplayAbility::UseAbility_Implementation()
{
    UAbilitySystemComponent* pASC = GetAbilitySystemComponentFromActorInfo();
    if (pASC)
    {
        pASC->TryActivateAbilityByClass(GetClass());
    }
}

bool URPGGameplayAbility::AbilityIsActive()
{
    return IsActive();
}

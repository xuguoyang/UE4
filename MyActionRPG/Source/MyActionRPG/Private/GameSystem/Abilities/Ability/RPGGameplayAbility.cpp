// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Abilities/Ability/RPGGameplayAbility.h"
#include "GameSystem/Roles/RPGCharacterBase.h"
#include "GameSystem/Abilities/RPGAbilitySystemComponent.h"

URPGGameplayAbility::URPGGameplayAbility()
{
    
}

TArray<FActiveGameplayEffectHandle> URPGGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel /*= -1*/)
{
    // ֻ�����ڷ����ִ��
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
    // ��ȡ����ϵͳ�������
    AActor* OwningActor = GetOwningActorFromActorInfo();
    ARPGCharacterBase* OwningCharacter = Cast<ARPGCharacterBase>(OwningActor);
    URPGAbilitySystemComponent* OwningASC = URPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);
    if (OwningASC)
    {
        // ���������һ��Ŀ�����ͣ�ִ�������߼���ʵ��Ŀ������ں��������
        if (Contanier.TargetType.Get())
        {
            TArray<FHitResult> HitResults;          // �����ײ����ȡ����Ŀ��
            TArray<AActor*> TargetActors;           // ��Χ���ܻ�ȡ����Ŀ�꣬������

            // ͨ�����õ�TargetType�����͵�CDO��AddTargets������ȡHitResults��TargetActors
            // ͨ�������EventData����ȷ��Ŀ��Actor
            const URPGTargetType* TargetTypeCDO = Contanier.TargetType.GetDefaultObject();
            AActor* AvatarActor = GetAvatarActorFromActorInfo();
            TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
            ReturnSpec.AddTargets(HitResults, TargetActors);
        }

        if (OverrideGameplayLevel == INDEX_NONE)
        {
            OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
        }

        // ����GameplayEffectSpecs
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

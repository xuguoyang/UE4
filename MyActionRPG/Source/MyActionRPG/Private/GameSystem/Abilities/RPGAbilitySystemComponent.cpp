// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAbilitySystemComponent.h"
#include "RPGCharacterBase.h"
#include "RPGGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "RPGAttributeSet.h"
#include "GameplayCueManager.h"

URPGAbilitySystemComponent::URPGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer)
{
    bAbilitiesInitialized = false;
}

URPGAbilitySystemComponent* URPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
    return Cast<URPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

void URPGAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URPGGameplayAbility*>& ActiveAbilities)
{
    TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

    // Iterate the list of all ability specs
    for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
    {
        // Iterate all instances on this ability spec
        TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

        for (UGameplayAbility* ActiveAbility : AbilityInstances)
        {
            ActiveAbilities.Add(Cast<URPGGameplayAbility>(ActiveAbility));
        }
    }
}

void URPGAbilitySystemComponent::RefreshAllAttribute()
{
    // 移除之前的技能和属性
    RemoveBaseAttributeGE();
    RemoveBaseAbility();

    // 重新添加技能和属性
    AddBaseAttributeGE();
    AddBaseAbility();

    // 发送技能属性刷新的通知
}

bool URPGAbilitySystemComponent::SetAbilityLevelByClass(TSubclassOf<URPGGameplayAbility>& AbilityClass, int32 TargetLevel)
{
    bool bSuccess = false;

    const URPGGameplayAbility* InAbilityCDO = Cast<URPGGameplayAbility>(AbilityClass.GetDefaultObject());
    if (!InAbilityCDO)
    {
        return false;
    }

    for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
    {
        // 角色拥有是否拥有此技能
        if (Spec.Ability == InAbilityCDO &&
            TargetLevel != Spec.Level    && 
            InAbilityCDO->GAStaticData.AbilityApplyType == EAbilityApplyType::AbilityApplyType_Passive)
        {
            Spec.Level = TargetLevel;

            // 先移除这个技能所有的Effect
            //InAbilityCDO->RemoveAllEffect();
           

           // 重新添加这个技能所有的Effect
            //InAbilityCDO->ApplyEffectContainer()
            break;
        }
    }

    return bSuccess;
}

bool URPGAbilitySystemComponent::UpdateAbilityAndAttributeByLevel(int32 TargetLevel)
{
    return true;
}

void URPGAbilitySystemComponent::ExecuteGamePlayCueInSever(const FGameplayTag GameplayCueTag)
{
    ExecuteGameplayCue(GameplayCueTag);
}

void URPGAbilitySystemComponent::ExecuteGamePlayCueInLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameter)
{
    UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Executed, Parameter);
}

void URPGAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld()->GetNetMode() != ENetMode::NM_Client && !bAbilitiesInitialized)
    {
        InitAttribute();
        InitAbility();
    }
}

void URPGAbilitySystemComponent::InitAttribute()
{
    for (UAttributeSet* set :SpawnedAttributes)
    {
        URPGAttributeSet* RPGSet = Cast<URPGAttributeSet>(set);
        if (RPGSet)
        {
            RPGSet->InitAttributeSet();
        }
    }

    AddBaseAttributeGE();
}

void URPGAbilitySystemComponent::InitAbility()
{
    AddBaseAbility();

    bAbilitiesInitialized = true;
    NotifyAbilityInitialized();
}

void URPGAbilitySystemComponent::AddBaseAttributeGE()
{
    if (BasicAttributeEffectClass == nullptr)
    {
        return;
    }

    ActiveBaseGEs.Empty(ActiveBaseGEs.Num());
    FGameplayEffectContextHandle EffectContext = MakeEffectContext();
    EffectContext.AddSourceObject(this->GetOwner());

    FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(BasicAttributeEffectClass, 1, EffectContext);
    if (NewHandle.IsValid())
    {
        ActiveBaseGEs.Add(ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get()));
    }
}

void URPGAbilitySystemComponent::RemoveBaseAttributeGE()
{
    for(auto& Handle: ActiveBaseGEs)
    {
        RemoveActiveGameplayEffect(Handle);
    }
   
    ActiveBaseGEs.Empty(ActiveBaseGEs.Num());
}

void URPGAbilitySystemComponent::AddBaseAbility()
{
    ActiveBaseGEs.Empty(ActiveBaseGEs.Num());
    // 将配置的技能添加到技能系统中
    for (FAbilityClassData& Data : AbilityClasses)
    {
        if (Data.AbilityClass == nullptr)
        {
            continue;
        }

        // 添加技能到技能系统
        ActiveBaseGAs.Add(GiveAbility(FRPGGameplayAbilitySpc(Data.AbilityClass)));
    }
}

void URPGAbilitySystemComponent::RemoveBaseAbility()
{
    for (auto& AbilityHandle: ActiveBaseGAs)
    {
        ClearAbility(AbilityHandle);
    }

    ActiveBaseGAs.Empty(ActiveBaseGAs.Num());
}

void URPGAbilitySystemComponent::NotifyAbilityInitialized()
{

}

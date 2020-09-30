// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "RPGCharacterBase.h"
#include "GameFramework/Controller.h"


URPGAttributeSet::URPGAttributeSet()
    : Health(1.f)
    , MaxHealth(1.f)
    , Mana(0.f)
    , MaxMana(0.f)
    , AttackPower(1.0f)
    , DefensePower(1.0f)
    , MoveSpeed(1.0f)
    , Damage(0.0f)
{

}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeBaseChange(Attribute, NewValue);

    // MaxHealth和MaxMana的值发生变化需要刷新下对应的当前值
    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }
    else if (Attribute == GetMaxManaAttribute())
    {
        AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
    }
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    // 在此处理属性发生变化的后续处理
    Super::PostGameplayEffectExecute(Data);
    
    // 获取处理属性的相关对象
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* pSourceASC = Context.GetInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

    float DeltaValue = 0;
    if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
    {
        DeltaValue = Data.EvaluatedData.Magnitude;
    }

    // Target一般情况下都是当前AttributeSet的拥有actor
    AActor* pTargetActor = nullptr;
    AController* pTargetController = nullptr;
    ARPGCharacterBase* pTargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        pTargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        pTargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        pTargetCharacter = Cast<ARPGCharacterBase>(pTargetActor);
    }
    
    // 不同的属性做相应的处理
    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        // 获取SourceActor
        AActor* SourceActor = nullptr;
        AController* SourceController = nullptr;
        ARPGCharacterBase* SourceCharacter = nullptr;
        if (pSourceASC && pSourceASC->AbilityActorInfo.IsValid() && pSourceASC->AbilityActorInfo->AvatarActor.IsValid())
        {
            SourceActor = pSourceASC->AbilityActorInfo->AvatarActor.Get();
            SourceController = pSourceASC->AbilityActorInfo->PlayerController.Get();
            if (SourceController == nullptr && SourceActor != nullptr)
            {
                if (APawn* Pawn = Cast<APawn>(SourceActor))
                {
                    SourceController = Pawn->GetController();
                }
            }

            // Use the controller to find the source pawn
            if (SourceController)
            {
                SourceCharacter = Cast<ARPGCharacterBase>(SourceController->GetPawn());
            }
            else
            {
                SourceCharacter = Cast<ARPGCharacterBase>(SourceActor);
            }
        }

        FHitResult HitResult;
        if (Context.GetHitResult())
        {
            HitResult = *Context.GetHitResult();
        }

        const float LocalDamageDone = GetDamage();
        SetDamage(0.f);

        // 应用伤害
        if (LocalDamageDone > 0)
        {
            const float OldHealth = GetHealth();
            SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

            // 通知ARPGCharacterBase
            if (pTargetCharacter)
            {
                pTargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

                pTargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
            }
        }
    }  
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        if (pTargetCharacter)
        {
            // Call for all health changes
            pTargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
        }
    }
    else if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));

        if (pTargetCharacter)
        {
            pTargetCharacter->HandleManaChanged(DeltaValue, SourceTags);
        }
    }
    else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
    {
        if (pTargetCharacter)
        {
            pTargetCharacter->HandleMoveSpeedChanged(DeltaValue, SourceTags);
        }
    }
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPGAttributeSet, Health);
    DOREPLIFETIME(URPGAttributeSet, MaxHealth);
    DOREPLIFETIME(URPGAttributeSet, Mana);
    DOREPLIFETIME(URPGAttributeSet, MaxMana);
    DOREPLIFETIME(URPGAttributeSet, AttackPower);
    DOREPLIFETIME(URPGAttributeSet, DefensePower);
    DOREPLIFETIME(URPGAttributeSet, MoveSpeed);
}

void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    UAbilitySystemComponent* pAbilityComp = GetOwningAbilitySystemComponent();
    const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
    if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && pAbilityComp)
    {
        const float CurrentValue = AffectedAttribute.GetCurrentValue();
        float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

        pAbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
    }
}

void URPGAttributeSet::OnRep_Health()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health);
}

void URPGAttributeSet::OnRep_MaxHealth()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth);
}

void URPGAttributeSet::OnRep_Mana()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Mana);
}

void URPGAttributeSet::OnRep_MaxMana()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxMana);
}

void URPGAttributeSet::OnRep_AttackPower()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AttackPower);
}

void URPGAttributeSet::OnRep_DefensePower()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, DefensePower);
}

void URPGAttributeSet::OnRep_MoveSpeed()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MoveSpeed);
}

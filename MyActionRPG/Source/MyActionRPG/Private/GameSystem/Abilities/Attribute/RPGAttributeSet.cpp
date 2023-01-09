// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Abilities/Attribute/RPGAttributeSet.h"
#include <GameplayEffect.h>
#include <GameplayEffectExtension.h>
#include <AbilitySystemComponent.h>
#include "GameSystem/Roles/RPGCharacterBase.h"
#include <GameFramework/Controller.h>


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

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPGAttributeSet, Strength);
    DOREPLIFETIME(URPGAttributeSet, Agility);
    DOREPLIFETIME(URPGAttributeSet, Wisdom);
    DOREPLIFETIME(URPGAttributeSet, AttackInterval);
    DOREPLIFETIME(URPGAttributeSet, Health);
    DOREPLIFETIME(URPGAttributeSet, MaxHealth);
    DOREPLIFETIME(URPGAttributeSet, Mana);
    DOREPLIFETIME(URPGAttributeSet, MaxMana);
    DOREPLIFETIME(URPGAttributeSet, AttackPower);
    DOREPLIFETIME(URPGAttributeSet, DefensePower);
    DOREPLIFETIME(URPGAttributeSet, MoveSpeed);
}


void URPGAttributeSet::InitAttributeSet()
{
    BindFuctions();
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

void URPGAttributeSet::OnRep_Health(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, OldValue);
}

void URPGAttributeSet::OnRep_MaxHealth(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, OldValue);
}

void URPGAttributeSet::OnRep_Mana(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Mana, OldValue);
}

void URPGAttributeSet::OnRep_MaxMana(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxMana, OldValue);
}

void URPGAttributeSet::OnRep_AttackPower(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AttackPower, OldValue);
}

void URPGAttributeSet::OnRep_DefensePower(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, DefensePower, OldValue);
}

void URPGAttributeSet::OnRep_MoveSpeed(const FRPGAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MoveSpeed, OldValue);
}

void URPGAttributeSet::OnRep_AttackInterval(const FRPGAttributeData& OldValue)
{

}


void URPGAttributeSet::OnRep_Strength(const FRPGAttributeData& OldValue)
{

}

void URPGAttributeSet::OnRep_Agility(const FRPGAttributeData& OldValue)
{

}

void URPGAttributeSet::OnRep_Wisdom(const FRPGAttributeData& OldValue)
{

}



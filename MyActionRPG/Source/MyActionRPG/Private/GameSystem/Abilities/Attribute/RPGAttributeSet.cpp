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

void URPGAttributeSet::OnRep_AttackInterval()
{

}


void URPGAttributeSet::OnRep_Strength()
{

}

void URPGAttributeSet::OnRep_Agility()
{

}

void URPGAttributeSet::OnRep_Wisdom()
{

}



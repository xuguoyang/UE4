// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"


URPGAttributeSet::URPGAttributeSet()
{

}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPGAttributeSet, Health);
}

void URPGAttributeSet::OnRep_Health()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGGameplayAbility.h"
#include "RPGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
    static URPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

    void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URPGGameplayAbility*>& ActiveAbilities);
};

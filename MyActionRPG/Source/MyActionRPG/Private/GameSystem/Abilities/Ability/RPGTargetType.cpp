// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Abilities/Ability/RPGTargetType.h"
#include "GameSystem/Abilities/Ability/RPGGameplayAbility.h"
#include "GameSystem/Roles/RPGCharacterBase.h"


void URPGTargetType_UseOwner::GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
    OutActors.Add(TargetingCharacter);
}

void URPGTargetType_UseEventData::GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
    const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
    if (FoundHitResult)
    {
        OutHitResults.Add(*FoundHitResult);
    }
    else if (EventData.Target)
    {
        OutActors.Add(const_cast<AActor*>(EventData.Target));
    }
}

void URPGTargetType::GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{

}

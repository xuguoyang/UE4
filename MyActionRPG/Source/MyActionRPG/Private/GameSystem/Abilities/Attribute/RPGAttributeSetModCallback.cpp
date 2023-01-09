// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Abilities/Attribute/RPGAttributeSet.h"
#include <GameplayEffect.h>
#include <GameplayEffectExtension.h>
#include <AbilitySystemComponent.h>
#include "GameSystem/Roles/RPGCharacterBase.h"
#include <GameFramework/Controller.h>

void URPGAttributeSet::BindFuctions()
{
    Strength.BindFuctionPost(this, &URPGAttributeSet::PostChange_Strength);

    MaxHealth.BindFuctionAll(this, &URPGAttributeSet::PerChange_HealthMax, &URPGAttributeSet::PostChange_HealthMax);
    MaxMana.BindFuctionAll(this, &URPGAttributeSet::PerChange_ManaMax, &URPGAttributeSet::PostChange_ManaMax);
    Damage.BindFuctionPost(this, &URPGAttributeSet::PostChange_Damage);
    //MaxHealth.PerAttributeExecuteFuc = std::bind(&URPGAttributeSet::PerChange_HealthMax, this, std::placeholders::_1, std::placeholders::_2);
    //MaxHealth.PerAttributeExecuteFuc = std::mem_fn(&URPGAttributeSet::PerChange_HealthMax);
//    MaxHealth.PerAttributeExecuteFuc.swap(std::bind(&URPGAttributeSet::PerChange_HealthMax, this, std::placeholders::_1, std::placeholders::_2));
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeBaseChange(Attribute, NewValue);

    FRPGAttributeData* AttributeData = (FRPGAttributeData*)(Attribute.GetGameplayAttributeData(this));
    if (AttributeData != &MaxHealth)
    {
        return;
    }

    if (AttributeData->PerAttributeExecuteFuc)
    {
        AttributeData->PerAttributeExecuteFuc(Attribute, NewValue);
    }
}

struct PostGEData
{
    PostGEData(URPGAttributeSet* AttributeSet, const FGameplayEffectModCallbackData &ModData):
    Data(ModData),
    Context(Data.EffectSpec.GetContext()),
    SourceTags(*Data.EffectSpec.CapturedSourceTags.GetAggregatedTags()),
    InstigatorASC(nullptr),
    InstigatorCharacter(nullptr),
    SourceActor(nullptr),
    TargetASC(&ModData.Target),
    TargetCharacter(nullptr)
    {
        DeltaValue = 0;
        if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
        {
            DeltaValue = Data.EvaluatedData.Magnitude;
        }

        // Target一般情况下都是当前AttributeSet的拥有actor
        AActor*TargetActor = nullptr;
        if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
        {
            TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
            TargetCharacter = Cast<ARPGCharacterBase>(TargetActor);
        }

        // 获取施法者
        InstigatorASC = Context.GetInstigatorAbilitySystemComponent();
        AController* InstigatorController = nullptr;
        if (InstigatorASC && InstigatorASC->AbilityActorInfo.IsValid() && InstigatorASC->AbilityActorInfo->AvatarActor.IsValid())
        {
            SourceActor = InstigatorASC->AbilityActorInfo->AvatarActor.Get();
            InstigatorController = InstigatorASC->AbilityActorInfo->PlayerController.Get();
            if (InstigatorController == nullptr && SourceActor != nullptr)
            {
                if (APawn* Pawn = Cast<APawn>(SourceActor))
                {
                    InstigatorController = Pawn->GetController();
                }
            }

            // Use the controller to find the source pawn
            if (InstigatorController)
            {
                InstigatorCharacter = Cast<ARPGCharacterBase>(InstigatorController->GetPawn());
            }
            else
            {
                InstigatorCharacter = Cast<ARPGCharacterBase>(SourceActor);
            }
        }
    }
    const FGameplayEffectModCallbackData& Data;
    FGameplayEffectContextHandle Context;
    const FGameplayTagContainer& SourceTags;

    // 施法者
    UAbilitySystemComponent* InstigatorASC;
    ARPGCharacterBase*  InstigatorCharacter;
    AActor* SourceActor;

    // 施法目标
    UAbilitySystemComponent* TargetASC;
    ARPGCharacterBase* TargetCharacter;

    // 变化量
    float DeltaValue;
};
void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    // 在此处理属性发生变化的后续处理
    Super::PostGameplayEffectExecute(Data);

    FRPGAttributeData* AttributeData = (FRPGAttributeData*)(Data.EvaluatedData.Attribute.GetGameplayAttributeData(this));
    if (AttributeData == nullptr)
    {
        return;
    }

    if (AttributeData->PostAttributeExecuteFuc)
    {
        PostGEData GEData(this, Data);
        AttributeData->PostAttributeExecuteFuc(GEData);
    }
}


void URPGAttributeSet::PerChange_HealthMax(const FGameplayAttribute &Data, float& NewValue)
{
    AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
}

void URPGAttributeSet::PostChange_HealthMax(const PostGEData& Data)
{

}

void URPGAttributeSet::PerChange_ManaMax(const FGameplayAttribute &Data, float& NewValue)
{
    AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
}

void URPGAttributeSet::PostChange_ManaMax(const PostGEData& Data)
{

}

void URPGAttributeSet::PostChange_Damage(const PostGEData& Data)
{
    FHitResult HitResult;
    if (Data.Context.GetHitResult())
    {
        HitResult = *Data.Context.GetHitResult();
    }

    const float LocalDamageDone = GetDamage();
    SetDamage(0.f);

    // 应用伤害
    if (LocalDamageDone > 0)
    {
        const float OldHealth = GetHealth();
        SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

        // 通知ARPGCharacterBase
        if (Data.TargetCharacter)
        {
            Data.TargetCharacter->HandleDamage(LocalDamageDone, HitResult, Data.SourceTags, Data.InstigatorCharacter, Data.SourceActor);
            Data.TargetCharacter->HandleHealthChanged(-LocalDamageDone, Data.SourceTags);
        }
    }
}


void URPGAttributeSet::PostChange_Strength(const PostGEData& Data)
{

}

void URPGAttributeSet::PostChange_Health(const PostGEData& Data)
{

}

void URPGAttributeSet::PostChange_Mana(const PostGEData& Data)
{

}


void URPGAttributeSet::PostChange_MoveSpeed(const PostGEData& Data)
{

}
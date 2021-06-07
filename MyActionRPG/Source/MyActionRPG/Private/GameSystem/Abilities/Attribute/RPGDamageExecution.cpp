// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGDamageExecution.h"
#include "RPGAttributeSet.h"
#include "AbilitySystemComponent.h"

struct RPGDamageStatic
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

    RPGDamageStatic()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, DefensePower, Target, false);

        DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, AttackPower, Source, true);

        DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Damage, Source, true);
    }
};

static const RPGDamageStatic& DamageStatics()
{
    static RPGDamageStatic DmgStatics;
    return DmgStatics;
};


URPGDamageExecution::URPGDamageExecution()
{
    RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void URPGDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    // 获取双方的actor对象
    UAbilitySystemComponent* pSourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* pTargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceActor = pSourceASC ? pSourceASC->AvatarActor : nullptr;
    AActor* TargetActor = pTargetASC ? pTargetASC->AvatarActor : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float DefensePower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);

    float AttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

    float Damage = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

    float DamageDone = Damage * AttackPower / DefensePower;
    if (DamageDone > 0)
    {
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
    }
}

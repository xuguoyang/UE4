// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPGDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
    URPGDamageExecution();

    // 计算实际的伤害值
    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

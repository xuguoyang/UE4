// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGItemDataTableRow.h"
#include "ItemBase.h"
#include "RPGBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    /** Returns true if this is being run from an editor preview */
    UFUNCTION(BlueprintPure, Category = Loading)
    static bool IsInEditor();

    UFUNCTION(BlueprintPure, Category = AssetData)
    static class UTableAssetCommon* GetTableAssetCommon();

	UFUNCTION(BlueprintPure, Category = AssetData)
	static UObject* LoadResByPath(const FString& Path);

	UFUNCTION(BlueprintPure)
	static const FRPGItemDataTableRow& GetItemConfig(const UItemBase* ItemObj);

    UFUNCTION(BlueprintPure, Category = Ability)
    static bool DoesEffectContainerSpecHaveEffects(const FRPGGameplayEffectContainerSpec& ContainerSpec);

    UFUNCTION(BlueprintPure, Category = Ability)
    static bool DoesEffectContainerSpecHaveTargets(const FRPGGameplayEffectContainerSpec& ContainerSpec);

    UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "HitResults,TargetActors"))
    static FRPGGameplayEffectContainerSpec AddTargetsToEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

    UFUNCTION(BlueprintCallable, Category = Ability)
    static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec);
};

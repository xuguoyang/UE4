// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/RPGBlueprintLibrary.h"
#include "GameAsset/TableDataAsset/TableAssetCommon.h"
#include <UObject/SoftObjectPath.h>

bool URPGBlueprintLibrary::IsInEditor()
{
    return GIsEditor;
}

UTableAssetCommon* URPGBlueprintLibrary::GetTableAssetCommon()
{
    return UTableAssetCommon::Get();
}

UObject* URPGBlueprintLibrary::LoadResByPath(const FString& Path)
{
	FSoftObjectPath SoftPath(Path);
	UObject* Obj = SoftPath.TryLoad();

	return Obj;
}

const FRPGItemDataTableRow& URPGBlueprintLibrary::GetItemConfig(const UItemBase* ItemObj)
{
	static FRPGItemDataTableRow EmptyItemData;
	if (!ItemObj)
	{
		return EmptyItemData;
	}
	return *ItemObj->ItemConfig;
}

bool URPGBlueprintLibrary::DoesEffectContainerSpecHaveEffects(const FRPGGameplayEffectContainerSpec& ContainerSpec)
{
    return ContainerSpec.HasValidEffects();
}

bool URPGBlueprintLibrary::DoesEffectContainerSpecHaveTargets(const FRPGGameplayEffectContainerSpec& ContainerSpec)
{
    return ContainerSpec.HasValidTargets();
}

FRPGGameplayEffectContainerSpec URPGBlueprintLibrary::AddTargetsToEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
    FRPGGameplayEffectContainerSpec NewSpec = ContainerSpec;
    NewSpec.AddTargets(HitResults, TargetActors);
    return NewSpec;
}

TArray<FActiveGameplayEffectHandle> URPGBlueprintLibrary::ApplyExternalEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec)
{
    TArray<FActiveGameplayEffectHandle> AllEffects;

    // Iterate list of gameplay effects
    for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
    {
        if (SpecHandle.IsValid())
        {
            // If effect is valid, iterate list of targets and apply to all
            for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
            {
                AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
            }
        }
    }
    return AllEffects;
}

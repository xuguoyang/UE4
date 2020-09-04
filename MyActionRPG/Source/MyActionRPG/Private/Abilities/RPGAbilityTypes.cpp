#include "Abilities/RPGAbilityTypes.h"

bool FRPGGameplayEffectContainerSpec::HasValidEffects() const
{
    return TargetGameplayEffectSpecs.Num() > 0;
}

bool FRPGGameplayEffectContainerSpec::HasValidTargets() const
{
    return TagetData.Num() > 0;
}

void FRPGGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
    // 添加单体碰撞技能目标数据
    for(const FHitResult& HitResult: HitResults)
    {
        FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
        TagetData.Add(NewData);
    }


   // 添加AOE技能目标数据
    if (TargetActors.Num()>0)
    {
        FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
        NewData->TargetActorArray.Append(TargetActors);
        TagetData.Add(NewData);
    }
}

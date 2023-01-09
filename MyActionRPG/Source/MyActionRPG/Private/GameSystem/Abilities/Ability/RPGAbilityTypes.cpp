#include "GameSystem/Abilities/Ability/RPGAbilityTypes.h"

bool FRPGGameplayEffectContainerSpec::HasValidEffects() const
{
    return TargetGameplayEffectSpecs.Num() > 0;
}

bool FRPGGameplayEffectContainerSpec::HasValidTargets() const
{
    return TargetData.Num() > 0;
}

void FRPGGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
    // ��ӵ�����ײ����Ŀ������
    for(const FHitResult& HitResult: HitResults)
    {
        FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
        TargetData.Add(NewData);
    }


   // ���AOE����Ŀ������
    if (TargetActors.Num()>0)
    {
        FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
        NewData->TargetActorArray.Append(TargetActors);
        TargetData.Add(NewData);
    }
}

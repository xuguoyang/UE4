// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTask_PlayMontageWaitEvent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.h"
#include "Animation/AnimInstance.h"


URPGTask_PlayMontageWaitEvent::URPGTask_PlayMontageWaitEvent(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    Rate = 1.f;
    bStopWhenAbilityEnds = true;
}

void URPGTask_PlayMontageWaitEvent::Activate()
{
    // 当前是否有技能对象
    if(Ability == nullptr)
    {
        return;
    }

    bool bPlayeedMontage = false;
    URPGAbilitySystemComponent* RPGAbilitySystemComponent = GetTargetASC();

    if (RPGAbilitySystemComponent)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            // 绑定技能事件标签对应的回调
            EventHandle = RPGAbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &URPGTask_PlayMontageWaitEvent::OnGameplayEvent));

            // 播放蒙太奇动画，并绑定相关回调
            if (RPGAbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
            {      
                if (ShouldBroadcastAbilityTaskDelegates() == false)
                {
                    return;
                }

                // 绑定技能被打断的回调
                CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &URPGTask_PlayMontageWaitEvent::OnAbilityCancelled);

                // 绑定蒙太奇的融出回调
                BlendingOutDelegate.BindUObject(this, &URPGTask_PlayMontageWaitEvent::OnMontageBlendingOut);
                AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate);

                // 绑定蒙太奇结束的回调
                MontageEndedDelegate.BindUObject(this, &URPGTask_PlayMontageWaitEvent::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);

                // 设置主玩家根动画缩放
                ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
                if (Character && (Character->Role == ROLE_Authority || 
                    (Character->Role == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
                {
                    Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
                }

                bPlayeedMontage = true;
            }
        }
        else
        {
            ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));
        }
    }
    else
    {
        ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"));
    }

    // 处理播放蒙太奇失败
    if (!bPlayeedMontage)
    {
        ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    // 设置任务状态为等待
    SetWaitingOnAvatar();
}

void URPGTask_PlayMontageWaitEvent::ExternalCancel()
{
    check(AbilitySystemComponent);

    OnAbilityCancelled();

    Super::ExternalCancel();
}

FString URPGTask_PlayMontageWaitEvent::GetDebugString() const
{
    // 获取任务设置的蒙太奇字符串和当前播放的蒙太奇字符串的组合
    UAnimMontage* PlayingMontage = nullptr;
    if (Ability)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
        if (AnimInstance)
        {
           PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
        }
    }
    return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void URPGTask_PlayMontageWaitEvent::OnDestroy(bool bAbilityEnded)
{
    // 停止蒙太奇播放以及清空各类代理回调
    if (Ability)
    {
        Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
        if (bAbilityEnded && bStopWhenAbilityEnds)
        {
            StopPlayingMontage();
        }
    }

    URPGAbilitySystemComponent* RPGAbilitySystemComponent = GetTargetASC();
    if (RPGAbilitySystemComponent)
    {
        RPGAbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
    }

    Super::OnDestroy(bAbilityEnded);
}

URPGTask_PlayMontageWaitEvent* URPGTask_PlayMontageWaitEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/)
{
    UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

    URPGTask_PlayMontageWaitEvent* MyObj = NewAbilityTask<URPGTask_PlayMontageWaitEvent>(OwningAbility, TaskInstanceName);
    MyObj->MontageToPlay = MontageToPlay;
    MyObj->EventTags = EventTags;
    MyObj->Rate = Rate;
    MyObj->StartSection = StartSection;
    MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
    MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

    return MyObj;
}

URPGAbilitySystemComponent* URPGTask_PlayMontageWaitEvent::GetTargetASC()
{
    return Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
}

bool URPGTask_PlayMontageWaitEvent::StopPlayingMontage()
{
    // 当前在播放蒙太奇动画，停止播放
    UAnimInstance* pAnimInstance = GetAnimInstance();
    if (pAnimInstance == nullptr)
    {
        return false;
    }

    if (AbilitySystemComponent && Ability)
    {
        // 当前是否在使用此技能，并且当前播放的蒙太奇是否匹配
        if (AbilitySystemComponent->GetAnimatingAbility() == Ability
            && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
        {
            FAnimMontageInstance* MontageInstance = pAnimInstance->GetActiveMontageInstance();
            if (MontageInstance)
            {
                // 解绑蒙太奇事件
                MontageInstance->OnMontageBlendingOutStarted.Unbind();
                MontageInstance->OnMontageEnded.Unbind();
            }

            // 停止播放
            AbilitySystemComponent->CurrentMontageStop();
            return true;
        }
    }

    return false;
}

UAnimInstance* URPGTask_PlayMontageWaitEvent::GetAnimInstance()
{
    if (Ability)
    {
        const FGameplayAbilityActorInfo* pActorInfo = Ability->GetCurrentActorInfo();
        return pActorInfo->GetAnimInstance();
    }

    return nullptr;
}

void URPGTask_PlayMontageWaitEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
    {
        if (Montage == MontageToPlay)
        {
            AbilitySystemComponent->ClearAnimatingAbility(Ability);

            ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
            if (Character && (Character->Role == ROLE_Authority ||
                (Character->Role == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
            {
                Character->SetAnimRootMotionTranslationScale(1.f);
            }
        }
    }

    // 通知蓝图
    if (bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
    else
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void URPGTask_PlayMontageWaitEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // 蒙太奇结束后要结束任务
    if (!bInterrupted)
    {
        // 通知蓝图
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    EndTask();
}

void URPGTask_PlayMontageWaitEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        FGameplayEventData TempData = *Payload;
        TempData.EventTag = EventTag;

        EventReceived.Broadcast(EventTag, TempData);
    }
}

void URPGTask_PlayMontageWaitEvent::OnAbilityCancelled()
{
    if (StopPlayingMontage())
    {
        // 通知蓝图
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

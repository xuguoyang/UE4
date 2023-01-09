// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Abilities/Task/RPGTask_PlayMontageWaitEvent.h"
#include <AbilitySystemGlobals.h>
#include <AbilitySystemComponent.h>
#include "GameSystem/Abilities/RPGAbilitySystemComponent.h"
#include <Animation/AnimInstance.h>


URPGTask_PlayMontageWaitEvent::URPGTask_PlayMontageWaitEvent(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    Rate = 1.f;
    bStopWhenAbilityEnds = true;
}

void URPGTask_PlayMontageWaitEvent::Activate()
{
    // ��ǰ�Ƿ��м��ܶ���
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
            // �󶨼����¼���ǩ��Ӧ�Ļص�
            EventHandle = RPGAbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &URPGTask_PlayMontageWaitEvent::OnGameplayEvent));

            // ������̫�涯����������ػص�
            if (RPGAbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
            {      
                if (ShouldBroadcastAbilityTaskDelegates() == false)
                {
                    return;
                }

                // �󶨼��ܱ���ϵĻص�
                CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &URPGTask_PlayMontageWaitEvent::OnAbilityCancelled);

                // ����̫����ڳ��ص�
                BlendingOutDelegate.BindUObject(this, &URPGTask_PlayMontageWaitEvent::OnMontageBlendingOut);
                AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate);

                // ����̫������Ļص�
                MontageEndedDelegate.BindUObject(this, &URPGTask_PlayMontageWaitEvent::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);

                // ��������Ҹ���������
                ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
                if (Character && (Character->GetLocalRole() == ROLE_Authority || 
                    (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
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

    // ��������̫��ʧ��
    if (!bPlayeedMontage)
    {
        ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    // ��������״̬Ϊ�ȴ�
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
    // ��ȡ�������õ���̫���ַ����͵�ǰ���ŵ���̫���ַ��������
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
    // ֹͣ��̫�沥���Լ���ո������ص�
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
    // ��ǰ�ڲ�����̫�涯����ֹͣ����
    UAnimInstance* pAnimInstance = GetAnimInstance();
    if (pAnimInstance == nullptr)
    {
        return false;
    }

    if (AbilitySystemComponent && Ability)
    {
        // ��ǰ�Ƿ���ʹ�ô˼��ܣ����ҵ�ǰ���ŵ���̫���Ƿ�ƥ��
        if (AbilitySystemComponent->GetAnimatingAbility() == Ability
            && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
        {
            FAnimMontageInstance* MontageInstance = pAnimInstance->GetActiveMontageInstance();
            if (MontageInstance)
            {
                // �����̫���¼�
                MontageInstance->OnMontageBlendingOutStarted.Unbind();
                MontageInstance->OnMontageEnded.Unbind();
            }

            // ֹͣ����
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
            if (Character && (Character->GetLocalRole() == ROLE_Authority ||
                (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
            {
                Character->SetAnimRootMotionTranslationScale(1.f);
            }
        }
    }

    // ֪ͨ��ͼ
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
    // ��̫�������Ҫ��������
    if (!bInterrupted)
    {
        // ֪ͨ��ͼ
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
        // ֪ͨ��ͼ
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

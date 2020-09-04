// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyActionRPG.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RPGTask_PlayMontageWaitEvent.generated.h"

class URPGAbilitySystemComponent;

/** 声明一个蒙太奇回调使用的多播代理 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRPGPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);
/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGTask_PlayMontageWaitEvent : public UAbilityTask
{
	GENERATED_BODY()

public:
   URPGTask_PlayMontageWaitEvent(const FObjectInitializer& ObjectInitializer);
   virtual void Activate() override;
   virtual void ExternalCancel() override;
   virtual FString GetDebugString() const override;
   virtual void OnDestroy(bool bAbilityEnded) override;

    /** 蒙太奇播放结束的回调代理*/
    UPROPERTY(BlueprintAssignable)
    FRPGPlayMontageAndWaitForEventDelegate OnCompleted;

    /** 蒙太奇开始融出的回调代理*/
    UPROPERTY(BlueprintAssignable)
    FRPGPlayMontageAndWaitForEventDelegate OnBlendOut;

    /** 蒙太奇被打断的回调代理*/
    UPROPERTY(BlueprintAssignable)
    FRPGPlayMontageAndWaitForEventDelegate OnInterrupted;

    /** 技能任务被其他技能取消的回调代理*/
    UPROPERTY(BlueprintAssignable)
    FRPGPlayMontageAndWaitForEventDelegate OnCancelled;

    /** 触发Gameplay事件的回调代理*/
    UPROPERTY(BlueprintAssignable)
    FRPGPlayMontageAndWaitForEventDelegate EventReceived;

    /** 技能任务工厂函数*/
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static URPGTask_PlayMontageWaitEvent* PlayMontageAndWaitForEvent(
        UGameplayAbility* OwningAbility,
        FName TaskInstanceName,
        UAnimMontage* MontageToPlay,
        FGameplayTagContainer EventTags,
        float Rate = 1.f,
        FName StartSection = NAME_None,
        bool bStopWhenAbilityEnds = true,
        float AnimRootMotionTranslationScale = 1.f);

private:
    /** 当前播放的蒙太奇*/
    UPROPERTY()
    UAnimMontage* MontageToPlay;
   
    /** 对应gameplay事件的tags列表*/
    UPROPERTY()
    FGameplayTagContainer EventTags;

    /** Playback rate*/
    UPROPERTY()
    float Rate;

    /** 蒙太奇开始播放的分段*/
    UPROPERTY()
    FName StartSection;

    UPROPERTY()
    float AnimRootMotionTranslationScale;

    /** 技能结束是否中断蒙太奇*/
    UPROPERTY()
    bool bStopWhenAbilityEnds;

    /** 获取技能组件*/
    URPGAbilitySystemComponent* GetTargetASC();

    /** 停止蒙太奇动画播放*/
    bool StopPlayingMontage();

    /** 获取AnimInstance*/
    UAnimInstance* GetAnimInstance();

    void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);
    void OnAbilityCancelled();
    /** 代理句柄*/
    FOnMontageBlendingOutStarted BlendingOutDelegate;
    FOnMontageEnded MontageEndedDelegate;
    FDelegateHandle CancelledHandle;
    FDelegateHandle EventHandle;
};

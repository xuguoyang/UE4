// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FAbilityClassData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<URPGGameplayAbility> AbilityClass;
};
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYACTIONRPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()  
public:
    URPGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);
public:
    static URPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

    void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URPGGameplayAbility*>& ActiveAbilities);

    /**ˢ���������� */
    void RefreshAllAttribute();

    /**���ü��ܵȼ�*/
    bool SetAbilityLevelByClass(TSubclassOf<URPGGameplayAbility>& AbilityClass, int32 TargetLevel);

    /**���ݵȼ����¼��ܺ�����*/
    bool UpdateAbilityAndAttributeByLevel(int32 TargetLevel);

    /** ִ��GamePlayCue�� Ч����ͬ�������й����Ŀͻ���*/
    UFUNCTION(BlueprintCallable, Category = GameplayCue)
    void ExecuteGamePlayCueInSever(const FGameplayTag GameplayCueTag);

    /** �ڿͻ��˱���ִ��GamePlayCue*/
    UFUNCTION(BlueprintCallable, Category = GameplayCue)
    void ExecuteGamePlayCueInLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameter);

// ��������
public:
    /** ��������Ч��*/
    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameplayEffect> BasicAttributeEffectClass;

    /** ��������*/
    UPROPERTY(EditAnywhere)
    TArray<FAbilityClassData> AbilityClasses;

    UPROPERTY()
    int32 bAbilitiesInitialized;

private:
    virtual void BeginPlay() override;

private:
    /** ��ʼ����ɫ����*/
    void InitAttribute();

    /** ��ʼ������*/
    void InitAbility();

    /** ��ӻ�������*/
    void AddBaseAttributeGE();

    /** �Ƴ���������*/
    void RemoveBaseAttributeGE();

    /** ��ӻ�������*/
    void AddBaseAbility();

    /** �Ƴ���������*/
    void RemoveBaseAbility();
// ��������֪ͨ
private:
    void NotifyAbilityInitialized();

private:
    TArray<FGameplayAbilitySpecHandle> ActiveBaseGAs;
    TArray<FActiveGameplayEffectHandle> ActiveBaseGEs;
};

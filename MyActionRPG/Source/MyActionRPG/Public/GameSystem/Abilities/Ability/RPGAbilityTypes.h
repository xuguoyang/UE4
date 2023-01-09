// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include "RPGTargetType.h"
#include "Engine/EngineTypes.h"
#include "RPGAbilityTypes.generated.h"


/** ��չFGameplayAbilitySpc�����ж�̬�����Լ�������*/
USTRUCT(BlueprintType)
struct FRPGGameplayAbilitySpc : public FGameplayAbilitySpec
{
    GENERATED_USTRUCT_BODY()
    
    FRPGGameplayAbilitySpc() :
    FGameplayAbilitySpec() {}

    FRPGGameplayAbilitySpc(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject* InSourceObject = nullptr):
    FGameplayAbilitySpec(InAbilityClass, InLevel, InInputID, InSourceObject)
    {}

    /** */
};

/** 
*   gameplay effect class�б�a tag, and targeting info
*  �����������ͼ��assets�б���̬����(ָ�����ͺ�Ĭ��ֵ)Ȼ��������ʱ��ת����Specs��ִ��
*/
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainer
{
    GENERATED_BODY()

public:
    FRPGGameplayEffectContainer() {}

    /**�趨����Ŀ������*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TSubclassOf<URPGTargetType> TargetType;

    /** Ӧ���ڵ�ǰĿ��ļ���Ч������ */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
   
};

/** ��ӦFRPGGameplayEffectContainer��������ִ�м���İ汾 */
USTRUCT(BlueprintType)
struct FRPGGameplayEffectContainerSpec
{
    GENERATED_BODY()
public:
    FRPGGameplayEffectContainerSpec() {}

    /** ����ϵͳ�ڲ�Ŀ������*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    FGameplayAbilityTargetDataHandle TargetData;

    /** Ӧ���ڵ�ǰĿ��ļ���Ч�������б� */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

    /** �Ƿ�����Ч��effect specs*/
    bool HasValidEffects() const;

    /** �Ƿ�����Ч��targets */
    bool HasValidTargets() const;

    /** ����Ŀ������*/
    void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};

/** ��������ö�� */
UENUM(BlueprintType)
enum class EAttackDisType: uint8
{
    AttackDisType_None,
    AttackDisType_Near,             // ��ս
    AttackDisType_Far               // Զ��
};

/** �ͷŷ�ʽ */
UENUM(BlueprintType)
enum class EAbilityApplyType:uint8
{
    AbilityApplyType_Actions,       // �����ͷ�
    AbilityApplyType_Passive        // ��������
};

/** ���ܵľ�̬�������� **/
USTRUCT(BlueprintType)
struct FGameplayAbilityStaticData
{
    GENERATED_BODY()

    /** ����ͼ��*/
    UPROPERTY(EditDefaultsOnly)
    FSoftObjectPath   Icon;

    /** �ͷŷ�ʽ��������������*/
    UPROPERTY(EditDefaultsOnly)
    EAttackDisType AttackDisType = EAttackDisType::AttackDisType_Near;

    /** �ͷž��루��ս��Զ�̣�*/
    UPROPERTY(EditDefaultsOnly)
    EAbilityApplyType AbilityApplyType = EAbilityApplyType::AbilityApplyType_Actions;

};
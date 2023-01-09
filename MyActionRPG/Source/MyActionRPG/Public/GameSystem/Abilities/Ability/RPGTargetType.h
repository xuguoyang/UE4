// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyActionRPG.h"
#include "UObject/NoExportTypes.h"
#include "RPGTargetType.generated.h"

/**
 * ���ڼ����ͷ�Ŀ���ɸѡ����
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class MYACTIONRPG_API URPGTargetType : public UObject
{
	GENERATED_BODY()
public:
    /** ��ȡ����Ч��Ӧ�õ�Ŀ��, ��ͼ���Ը��Ǵ˺��� */
    UFUNCTION(BlueprintNativeEvent)
    void GetTargets(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};


/** ������ʩ�ӵļ���*/
UCLASS(NotBlueprintable)
class MYACTIONRPG_API URPGTargetType_UseOwner:public URPGTargetType
{
    GENERATED_BODY()
public:
    virtual void GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** ͨ�������¼���ȡĿ�� */
UCLASS(NotBlueprintable)
class MYACTIONRPG_API URPGTargetType_UseEventData :public URPGTargetType
{
    GENERATED_BODY()
public:
    virtual void GetTargets_Implementation(ARPGCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

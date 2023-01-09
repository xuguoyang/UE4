// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyActionRPG.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include <functional>
#include "RPGAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


struct PostGEData;
USTRUCT(BlueprintType)
struct MYACTIONRPG_API FRPGAttributeData : public FGameplayAttributeData
{
    GENERATED_BODY()
    FRPGAttributeData():FGameplayAttributeData(),
    PerAttributeExecuteFuc(nullptr),
    PostAttributeExecuteFuc(nullptr) 
    {}

    FRPGAttributeData(float DefaultValue):FGameplayAttributeData(DefaultValue),
    PerAttributeExecuteFuc(nullptr),
    PostAttributeExecuteFuc(nullptr)
    {
    }
    
    typedef std::function<void(const FGameplayAttribute&, float&)> PerGEExecuteFuc;
    typedef std::function<void(const PostGEData&)> PostGEExecuteFuc;

    template<class _Fx1, class _Fx2>
    void BindFuctionAll(class URPGAttributeSet* Obj, _Fx1&& _Func1, _Fx2&& _Func2)
    {
          PerAttributeExecuteFuc = std::bind(_Func1, Obj, std::placeholders::_1, std::placeholders::_2);
          PostAttributeExecuteFuc = std::bind(_Func2, Obj, std::placeholders::_1);
    }

    template<class _Fx1>
    void BindFuctionPer(class URPGAttributeSet* Obj, _Fx1&& _Func1)
    {
        PerAttributeExecuteFuc = std::bind(_Func1, Obj, std::placeholders::_1, std::placeholders::_2);
    }

    template<class _Fx1>
    void BindFuctionPost(class URPGAttributeSet* Obj, _Fx1&& _Func1)
    {
        PostAttributeExecuteFuc = std::bind(_Func1, Obj, std::placeholders::_1);
    }
    
    PerGEExecuteFuc PerAttributeExecuteFuc;
    PostGEExecuteFuc PostAttributeExecuteFuc;
};


/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
    URPGAttributeSet();

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void InitAttributeSet();
    // ����ֵ
    UPROPERTY(BlueprintReadOnly, Category = "Base", ReplicatedUsing = OnRep_Strength)
    FRPGAttributeData Strength;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Strength)

    // ����ֵ
    UPROPERTY(BlueprintReadOnly, Category = "Base", ReplicatedUsing = OnRep_Agility)
    FRPGAttributeData Agility;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Agility)

    // ����ֵ
    UPROPERTY(BlueprintReadOnly, Category = "Base", ReplicatedUsing = OnRep_Wisdom)
    FRPGAttributeData Wisdom;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Wisdom)

    // ��ɫHP����
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FRPGAttributeData Health;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Health)

    // MaxHealth
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
    FRPGAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth)

    // Mana,��ǰ����
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
    FRPGAttributeData Mana;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Mana)

    // MaxMana����ǰ�������
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
    FRPGAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxMana)

    // AttackPower�������������ڼ���Damage
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
    FRPGAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, AttackPower)

    // DefensePower������ֵ�����ڼ��㱻Damage
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
    FRPGAttributeData DefensePower;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, DefensePower)

    // �������ʱ��
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackInterval)
    FRPGAttributeData AttackInterval;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, AttackInterval)

    // MoveSpeed����ɫ���ƶ��ٶ�
    UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
    FRPGAttributeData MoveSpeed;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, MoveSpeed)

    // Damage, ��ǰ�Լ��ܵ����˺�
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FRPGAttributeData Damage;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Damage)
protected:
    // �������ֵ�����仯��Ҫˢ���¶�Ӧ�ĵ�ǰֵ
    void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

    // �ͻ������Իص�
protected:
    UFUNCTION()
    void OnRep_Strength(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Agility(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Wisdom(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Health(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxHealth(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Mana(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxMana(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_AttackPower(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_DefensePower(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MoveSpeed(const FRPGAttributeData& OldValue);

    UFUNCTION()
    void OnRep_AttackInterval(const FRPGAttributeData& OldValue);

    // ��������
private:
    void BindFuctions();

    // Strength�ı�ʱ�Ļص�
    void PostChange_Strength(const PostGEData& Data);

    // Health�ı�ʱ�Ļص�
    void PostChange_Health(const PostGEData& Data);

    // HealthMax�ı�ʱ�Ļص�
    void PerChange_HealthMax(const FGameplayAttribute &Data, float& NewValue);
    void PostChange_HealthMax(const PostGEData& Data);

    // Mana�ı�ʱ�Ļص�
    void PostChange_Mana(const PostGEData& Data);

    // ManaMax�ı�ʱ�Ļص�
    void PerChange_ManaMax(const FGameplayAttribute &Data, float& NewValue);
    void PostChange_ManaMax(const PostGEData& Data);

    // Damage�ı�ʱ�Ļص�
    void PostChange_Damage(const PostGEData& Data);

    // Speed�ı�ʱ�Ļص�
    void PostChange_MoveSpeed(const PostGEData& Data);
};

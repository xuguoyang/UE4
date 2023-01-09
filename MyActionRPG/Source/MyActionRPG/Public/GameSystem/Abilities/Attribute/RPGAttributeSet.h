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
    // 力量值
    UPROPERTY(BlueprintReadOnly, Category = "Base", ReplicatedUsing = OnRep_Strength)
    FRPGAttributeData Strength;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Strength)

    // 敏捷值
    UPROPERTY(BlueprintReadOnly, Category = "Base", ReplicatedUsing = OnRep_Agility)
    FRPGAttributeData Agility;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Agility)

    // 智力值
    UPROPERTY(BlueprintReadOnly, Category = "Base", ReplicatedUsing = OnRep_Wisdom)
    FRPGAttributeData Wisdom;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Wisdom)

    // 角色HP属性
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FRPGAttributeData Health;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Health)

    // MaxHealth
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
    FRPGAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth)

    // Mana,当前蓝量
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
    FRPGAttributeData Mana;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Mana)

    // MaxMana，当前最大蓝量
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
    FRPGAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxMana)

    // AttackPower，攻击力，用于计算Damage
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
    FRPGAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, AttackPower)

    // DefensePower，防御值，用于计算被Damage
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
    FRPGAttributeData DefensePower;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, DefensePower)

    // 攻击间隔时间
    UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackInterval)
    FRPGAttributeData AttackInterval;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, AttackInterval)

    // MoveSpeed，角色的移动速度
    UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
    FRPGAttributeData MoveSpeed;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, MoveSpeed)

    // Damage, 当前自己受到的伤害
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FRPGAttributeData Damage;
    ATTRIBUTE_ACCESSORS(URPGAttributeSet, Damage)
protected:
    // 部分最大值发生变化需要刷新下对应的当前值
    void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

    // 客户端属性回调
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

    // 处理属性
private:
    void BindFuctions();

    // Strength改变时的回调
    void PostChange_Strength(const PostGEData& Data);

    // Health改变时的回调
    void PostChange_Health(const PostGEData& Data);

    // HealthMax改变时的回调
    void PerChange_HealthMax(const FGameplayAttribute &Data, float& NewValue);
    void PostChange_HealthMax(const PostGEData& Data);

    // Mana改变时的回调
    void PostChange_Mana(const PostGEData& Data);

    // ManaMax改变时的回调
    void PerChange_ManaMax(const FGameplayAttribute &Data, float& NewValue);
    void PostChange_ManaMax(const PostGEData& Data);

    // Damage改变时的回调
    void PostChange_Damage(const PostGEData& Data);

    // Speed改变时的回调
    void PostChange_MoveSpeed(const PostGEData& Data);
};

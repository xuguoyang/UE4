// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RPGType.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "RPGCharacterBase.generated.h"

class URPGAbilitySystemComponent;
class URPGAttributeSet;

UCLASS()
class MYACTIONRPG_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase();
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

    UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Returns current health, will be 0 if dead */
    UFUNCTION(BlueprintCallable)
    virtual float GetHealth() const;

    /** Returns maximum health, health will never be greater than this */
    UFUNCTION(BlueprintCallable)
    virtual float GetMaxHealth() const;

    /** Returns current mana */
    UFUNCTION(BlueprintCallable)
    virtual float GetMana() const;

    /** Returns maximum mana, mana will never be greater than this */
    UFUNCTION(BlueprintCallable)
    virtual float GetMaxMana() const;

    /** Returns current movement speed */
    UFUNCTION(BlueprintCallable)
    virtual float GetMoveSpeed() const;

    /** 获取角色等级*/
    UFUNCTION(BlueprintCallable)
    virtual int32 GetCharacterLevel() const;

    UFUNCTION(BlueprintCallable)
    virtual bool SetCharacterLevel(int32 NewLevel);
   
    /** 获取角色当前使用的武器*/
    UFUNCTION(BlueprintPure, Category = Equip)
    AActor* GetWeaponActor();

    /** 是否存活*/
    UFUNCTION(BlueprintPure)
    bool IsAlive();

    /** 死亡*/
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DelayDeath();

// 向服务器发消息
public:
    UFUNCTION(Server, Reliable, BlueprintCallable, WithValidation)
    void ServerEquipItem(UItemBase* Item);

// 向客户端发消息
public:

    // 角色技能，物品快捷栏
public:
    /** 技能组件*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    URPGAbilitySystemComponent* AbilitySystemComponent;

    /** 装备栏组件*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UItemEquipComponent* EquipComponent;

    /** 技能快捷栏组件*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UShortcutComponent* ShortcutComponent;

    // 角色等级
    UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
    int32 CharacterLevel;

    /** 技能系统使用的属性集,添加为拥有技能系统组件的Actor子对象后会自动注册给技能系统*/
    UPROPERTY()
    URPGAttributeSet* AttributeSet;

    
protected:
    /**导出处理伤害的蓝图事件*/
    UFUNCTION(BlueprintImplementableEvent)
    void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser);

    /**导出生命值发生改变的蓝图事件*/
    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser);
    virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    friend URPGAttributeSet;
};

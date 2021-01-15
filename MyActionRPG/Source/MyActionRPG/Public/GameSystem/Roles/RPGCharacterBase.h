// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RPGType.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/RPGGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "RPGInventoryInterface.h"
#include "RoleAssetBase.h"
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

    /** 释放一个装备好的技能*/
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool ActivateAbilitiesWithItemSlot(FRPGItemSlot ItemSlot, bool bAllowRemoteActivation = true);

    /** 获取装备好的技能列表*/
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void GetActiveAbilitiesWithItemSlot(FRPGItemSlot ItemSlot, TArray<URPGGameplayAbility*>& ActiveAbilities);

    /** 根据tag激活技能*/
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

    /** 根据tag获取当前激活的技能*/
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGGameplayAbility*>& ActiveAbilities);

    void SetRoleAsset(URoleAssetBase* ptr);

    template<typename T>
    T* GetRoleAsset()
    {
        if (!RoleAssetPtr.IsValid())
        {
            return nullptr;
        }

        return Cast<T>(RoleAssetPtr.Get());
    }
protected:
    // 装备栏切换道具触发的回调
    void OnItemSlotChanged(FRPGItemSlot ItemSlot, URPGItem* Item);
    void RefreshSlottedGameplayAbilities();

    // 添加玩家技能列表
    void AddStartupGameplayAbilities();

    // 添加装备栏中的技能
    void AddSlottedGameplayAbilities();
    void FillSlottedAbilitySpecs(TMap<FRPGItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs);

    UFUNCTION(BlueprintImplementableEvent)
    void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser);

    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    UFUNCTION(BlueprintImplementableEvent)
    void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    UFUNCTION(BlueprintImplementableEvent)
    void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser);
    virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
    virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
    virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    friend URPGAttributeSet;
    
protected:
    /** 技能组件*/
    UPROPERTY()
    URPGAbilitySystemComponent* AbilitySystemComponent;

    // 角色等级
    UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
    int32 CharacterLevel;

    /** 技能系统使用的属性集,添加为拥有技能系统组件的Actor子对象后会自动注册给技能系统*/
    UPROPERTY()
    URPGAttributeSet* AttributeSet;

    /** 玩家创建时，可以授予玩家的技能配置，可以通过tag、event等激活*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
    TArray<TSubclassOf<URPGGameplayAbility>>    GameplayAbilities;

    /** 技能装备栏对应的技能配置，跟装备栏绑定*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
    TMap<FRPGItemSlot, TSubclassOf<URPGGameplayAbility>> DefaultSlottedAbilities;

    /** 装备栏中装备的技能对象列表*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
    TMap<FRPGItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;

    /** 角色创建时添加被动技能*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
    TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

    // 装备更新的回调代理对象句柄
    FDelegateHandle InventoryUpdateHandle;
    FDelegateHandle InventoryLoadedHandle;

    /**获取角色当前装备栏数据的接口 */
    UPROPERTY()
    TScriptInterface<IRPGInventoryInterface> InventorySource;

    UPROPERTY()
    int32 bAbilitiesInitialized;

    // 当前actor对应的配置
    TWeakObjectPtr<URoleAssetBase> RoleAssetPtr;
};

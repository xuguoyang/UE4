// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RPGType.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameSystem/Abilities/Ability/RPGGameplayAbility.h"
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

    /** ��ȡ��ɫ�ȼ�*/
    UFUNCTION(BlueprintCallable)
    virtual int32 GetCharacterLevel() const;

    UFUNCTION(BlueprintCallable)
    virtual bool SetCharacterLevel(int32 NewLevel);
   
    /** ��ȡ��ɫ��ǰʹ�õ�����*/
    UFUNCTION(BlueprintPure, Category = Equip)
    AActor* GetWeaponActor();

    /** �Ƿ���*/
    UFUNCTION(BlueprintPure)
    bool IsAlive();

    /** ����*/
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DelayDeath();

// �����������Ϣ
public:
    UFUNCTION(Server, Reliable, BlueprintCallable, WithValidation)
    void ServerEquipItem(UItemBase* Item);

// ��ͻ��˷���Ϣ
public:

    // ��ɫ���ܣ���Ʒ�����
public:
    /** �������*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    URPGAbilitySystemComponent* AbilitySystemComponent;

    /** װ�������*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UItemEquipComponent* EquipComponent;

    /** ���ܿ�������*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UShortcutComponent* ShortcutComponent;

    // ��ɫ�ȼ�
    UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
    int32 CharacterLevel;

    /** ����ϵͳʹ�õ����Լ�,���Ϊӵ�м���ϵͳ�����Actor�Ӷ������Զ�ע�������ϵͳ*/
    UPROPERTY()
    URPGAttributeSet* AttributeSet;

    
protected:
    /**���������˺�����ͼ�¼�*/
    UFUNCTION(BlueprintImplementableEvent)
    void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser);

    /**��������ֵ�����ı����ͼ�¼�*/
    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser);
    virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    friend URPGAttributeSet;
};

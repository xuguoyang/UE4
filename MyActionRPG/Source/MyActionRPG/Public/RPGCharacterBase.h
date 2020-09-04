// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RPGType.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
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

    UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    /** 技能组件*/
    UPROPERTY()
    URPGAbilitySystemComponent* AbilitySystemComponent;

    // 角色等级

    /** 技能系统使用的属性集,添加为拥有技能系统组件的Actor子对象后会自动注册给技能系统*/
    UPROPERTY()
    URPGAttributeSet* AttributeSet;

    // 装备更新的回调
    FDelegateHandle InventoryUpdateHandle;
    FDelegateHandle InventoryLoadedHandle;
};

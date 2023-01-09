// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Roles/RPGCharacterBase.h"
#include "GameSystem/Abilities/Attribute/RPGAttributeSet.h"
#include <AbilitySystemComponent.h>
#include "GameSystem/Abilities/RPGAbilitySystemComponent.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
    // 创建技能组件
    AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

    // 添加装备栏组件
    EquipComponent = CreateDefaultSubobject<UItemEquipComponent>(TEXT("EquipComponent"));
    EquipComponent->SetIsReplicated(true);

    // 添加快捷栏组件
    ShortcutComponent = CreateDefaultSubobject<UShortcutComponent>(TEXT("ShortcutComponent"));
    ShortcutComponent->SetIsReplicated(true);

   // bAbilitiesInitialized = false;
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // 初始化技能
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

void ARPGCharacterBase::UnPossessed()
{

}

void ARPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPGCharacterBase, CharacterLevel);
}

void ARPGCharacterBase::BeginPlay()
{
    Super::BeginPlay();

   
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

// Called to bind functionality to input
void ARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ARPGCharacterBase::GetHealth() const
{
    if (!AttributeSet)
        return 1.f;

    return AttributeSet->GetHealth();
}

float ARPGCharacterBase::GetMaxHealth() const
{
    return AttributeSet->GetMaxHealth();
}

float ARPGCharacterBase::GetMana() const
{
    return AttributeSet->GetMana();
}

float ARPGCharacterBase::GetMaxMana() const
{
    return AttributeSet->GetMaxMana();
}

float ARPGCharacterBase::GetMoveSpeed() const
{
    return AttributeSet->GetMoveSpeed();
}

int32 ARPGCharacterBase::GetCharacterLevel() const
{
    return CharacterLevel;
}

bool ARPGCharacterBase::SetCharacterLevel(int32 NewLevel)
{
    if (CharacterLevel != NewLevel && NewLevel > 0)
    {
        // Our level changed so we need to refresh abilities
        //RemoveStartupGameplayAbilities();
        CharacterLevel = NewLevel;

        return true;
    }
    return false;
}

AActor* ARPGCharacterBase::GetWeaponActor()
{
    check(EquipComponent)

    UItemBase* WeaponItem = EquipComponent->FindItemByType(EItemEquipType::ItemEquipType_Weapon);
    if (WeaponItem != nullptr)
    {
        return WeaponItem->ItemActor;
    }

    return nullptr;
}

bool ARPGCharacterBase::IsAlive()
{
    return GetHealth() > 0.0f;
}

void ARPGCharacterBase::DelayDeath_Implementation()
{
    Destroy();
}

void ARPGCharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
    OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ARPGCharacterBase::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
    OnHealthChanged(DeltaValue, EventTags);
}

void ARPGCharacterBase::ServerEquipItem_Implementation(UItemBase* Item)
{
    UItemEquipComponent* IEC = FindComponentByClass<UItemEquipComponent>();
    if (IEC)
    {
        IEC->EquipItem(Item);
    }
}

bool ARPGCharacterBase::ServerEquipItem_Validate(UItemBase* Item)
{
    if (Item == nullptr)
    {
        return false;
    }

    // 查询背包中是否有此道具
    UItemContainerComponent* ICC = FindComponentByClass<UItemContainerComponent>();
    if (ICC != nullptr)
    {
        return ICC->HasItem(Item);
    }

    return true;
}

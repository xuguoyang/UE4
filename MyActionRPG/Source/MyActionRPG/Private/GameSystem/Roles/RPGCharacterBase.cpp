// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterBase.h"
#include "RPGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "RPGItem.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
    // 创建技能组件
    AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

    bAbilitiesInitialized = false;
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // 设置inventory source
    InventorySource = NewController;

    if (InventorySource != nullptr)
    {
        InventoryUpdateHandle = InventorySource->GetSlottedItemChangedDelegate().AddUObject(this, &ARPGCharacterBase::OnItemSlotChanged);
        InventoryLoadedHandle = InventorySource->GetInventoryLoadedDelegate().AddUObject(this, &ARPGCharacterBase::RefreshSlottedGameplayAbilities);
    }

    // 初始化技能
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
        AddStartupGameplayAbilities();
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
        AddStartupGameplayAbilities();

        return true;
    }
    return false;
}

bool ARPGCharacterBase::ActivateAbilitiesWithItemSlot(FRPGItemSlot ItemSlot, bool bAllowRemoteActivation /*= true*/)
{
    FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);
    if (FoundHandle && AbilitySystemComponent)
    {
        return AbilitySystemComponent->TryActivateAbility(*FoundHandle, bAllowRemoteActivation);
    }

    return false;
}

void ARPGCharacterBase::GetActiveAbilitiesWithItemSlot(FRPGItemSlot ItemSlot, TArray<URPGGameplayAbility*>& ActiveAbilities)
{
    FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

    if (FoundHandle && AbilitySystemComponent)
    {
        FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(*FoundHandle);

        if (FoundSpec)
        {
            TArray<UGameplayAbility*> AbilityInstances = FoundSpec->GetAbilityInstances();

            // Find all ability instances executed from this slot
            for (UGameplayAbility* ActiveAbility : AbilityInstances)
            {
                ActiveAbilities.Add(Cast<URPGGameplayAbility>(ActiveAbility));
            }
        }
    }
}

bool ARPGCharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation /*= true*/)
{
    if (AbilitySystemComponent)
    {
        return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
    }

    return false;
}

void ARPGCharacterBase::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGGameplayAbility*>& ActiveAbilities)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
    }
}

void ARPGCharacterBase::SetRoleAsset(URoleAssetBase* ptr)
{
    RoleAssetPtr = ptr;
}

void ARPGCharacterBase::OnItemSlotChanged(FRPGItemSlot ItemSlot, URPGItem* Item)
{
    RefreshSlottedGameplayAbilities();
}

void ARPGCharacterBase::RefreshSlottedGameplayAbilities()
{
    if (bAbilitiesInitialized)
    {
        // Refresh any invalid abilities and adds new ones
        //RemoveSlottedGameplayAbilities(false);
        AddSlottedGameplayAbilities();
    }
}

void ARPGCharacterBase::AddStartupGameplayAbilities()
{
    check(AbilitySystemComponent)

    // 该逻辑只跑在服务器上
    if(Role == ROLE_Authority && !bAbilitiesInitialized)
    {
        // 添加玩家的默认技能，不需要添加在装备栏里面
        for (TSubclassOf<URPGGameplayAbility>& StartupAbility: GameplayAbilities)
        {
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
        }

        // 给角色添加被动技能，直接应用GameplayEffect
        for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
        {
            // 构建effectcontext
            FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
            EffectContext.AddSourceObject(this);

            // 创建效果对象，并施加给角色
            FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
            if (NewHandle.IsValid())
            {
                FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
            }
        }
        // 添加装备栏中的技能
        AddSlottedGameplayAbilities();

        bAbilitiesInitialized = true;
    }
}

void ARPGCharacterBase::AddSlottedGameplayAbilities()
{
    TMap<FRPGItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
    FillSlottedAbilitySpecs(SlottedAbilitySpecs);

    // 填充SlottedAbilities
    for (const TPair<FRPGItemSlot, FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
    {
        // SlottedAbilitySpecs有而在SlottedAbilities中没有的添加
        FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);
        if (!SpecHandle.IsValid())
        {
            SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
        }
    }
}

void ARPGCharacterBase::FillSlottedAbilitySpecs(TMap<FRPGItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs)
{
    // 添加默认配置的技能
    for (const TPair<FRPGItemSlot, TSubclassOf<URPGGameplayAbility>>& DefaultPair : DefaultSlottedAbilities)
    {
        if (DefaultPair.Value.Get())
        {
            SlottedAbilitySpecs.Add(DefaultPair.Key, FGameplayAbilitySpec(DefaultPair.Value, GetCharacterLevel(), INDEX_NONE, this));
        }
    }

    // 添加装备栏中道具自带的技能
    if (InventorySource)
    {
        const TMap<FRPGItemSlot, URPGItem*>& SlottedItemMap = InventorySource->GetSlottedItemMap();

        // 获取每个道具的附加技能
        for (const TPair<FRPGItemSlot, URPGItem*>& SlottedItemPair : SlottedItemMap)
        {
            URPGItem* SlottedItem = SlottedItemPair.Value;

            // 道具如果是武器就使用武器的等级
            int32 AbilityLevel = GetCharacterLevel();
            if (SlottedItem && SlottedItem->ItemType.GetName() == FName(TEXT("Weapon")))
            {
                AbilityLevel = SlottedItem->AbilityLevel;
            }

            // 添加道具技能
            if (SlottedItem && SlottedItem->GrantedAbility)
            {
                SlottedAbilitySpecs.Add(SlottedItemPair.Key, FGameplayAbilitySpec(SlottedItem->GrantedAbility, AbilityLevel, INDEX_NONE, SlottedItem));
            }
        }      
    }
}

void ARPGCharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
    OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ARPGCharacterBase::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
    if (bAbilitiesInitialized)
    {
        OnHealthChanged(DeltaValue, EventTags);
    }
}

void ARPGCharacterBase::HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
    if (bAbilitiesInitialized)
    {
        OnManaChanged(DeltaValue, EventTags);
    }
}

void ARPGCharacterBase::HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
    GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

    if (bAbilitiesInitialized)
    {
        OnMoveSpeedChanged(DeltaValue, EventTags);
    }
}


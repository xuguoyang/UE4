// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableItem.h"

UUsableItem::UUsableItem()
{

}

bool UUsableItem::ShortcutUse()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (ASC)
    {
        return ASC->TryActivateAbility(BindAbilitySpecHandle);
    }

    return false;
}

bool UUsableItem::OnAddShortcut()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (ASC)
    {
        BindAbilitySpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(ItemConfig->AbilityClass));
        return BindAbilitySpecHandle.IsValid();
    }

    return false;
}

bool UUsableItem::OnRemoveShortcut()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (ASC)
    {
        ASC->ClearAbility(BindAbilitySpecHandle);
        return true;
    }

    return false;
}

EShortcutType UUsableItem::GetType()
{
    if (ItemConfig->ItemEquipType != EItemEquipType::ItemEquipType_None)
    {
        return EShortcutType::ShortcutGirdType_WeaponSkill;
    }

    return EShortcutType::ShortcutGirdType_Item;
}

UAbilitySystemComponent* UUsableItem::GetAbilitySystemComponent() const
{
    if (!OwnerActor)
    {
        return nullptr;
    }

    return OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
}

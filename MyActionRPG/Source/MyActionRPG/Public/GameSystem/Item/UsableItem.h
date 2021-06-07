// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShortcutUseInterface.h"
#include "UsableItem.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API UUsableItem : public UItemBase, public IShortcutUseInterface
{
	GENERATED_BODY()
public:
    UUsableItem();
    virtual bool ShortcutUse() override;
    virtual bool OnAddShortcut() override;
    virtual bool OnRemoveShortcut() override;
    virtual EShortcutType GetType()override;

private:
    UAbilitySystemComponent* GetAbilitySystemComponent() const;
   
};

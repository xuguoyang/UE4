// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayAbilitySpec.h"
#include "RPGItemTypes.h"
#include "ShortcutUseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShortcutUseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYACTIONRPG_API IShortcutUseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual bool ShortcutUse() = 0;
    virtual bool OnAddShortcut() = 0;
    virtual bool OnRemoveShortcut() = 0;
    virtual EShortcutType GetType() = 0;

protected:
    FGameplayAbilitySpecHandle BindAbilitySpecHandle;
};

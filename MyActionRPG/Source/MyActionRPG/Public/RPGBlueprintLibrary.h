// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API URPGBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    /**RPGItemSlot等于操作函数*/
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (RPGItemSlot)", CompactNodeTitle = "==", Keywords = "== equal"), Category = Inventory)
    static bool EqualEqual_RPGItemSlot(const FRPGItemSlot& A, const FRPGItemSlot& B);

    /** Returns true if this is being run from an editor preview */
    UFUNCTION(BlueprintPure, Category = Loading)
    static bool IsInEditor();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBlueprintLibrary.h"

bool URPGBlueprintLibrary::EqualEqual_RPGItemSlot(const FRPGItemSlot& A, const FRPGItemSlot& B)
{
    return A == B;
}

bool URPGBlueprintLibrary::IsInEditor()
{
    return GIsEditor;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBlueprintLibrary.h"
#include "TableAssetCommon.h"
#include "SoftObjectPath.h"

bool URPGBlueprintLibrary::EqualEqual_RPGItemSlot(const FRPGItemSlot& A, const FRPGItemSlot& B)
{
    return A == B;
}

bool URPGBlueprintLibrary::IsInEditor()
{
    return GIsEditor;
}

UTableAssetCommon* URPGBlueprintLibrary::GetTableAssetCommon()
{
    return UTableAssetCommon::Get();
}

UObject* URPGBlueprintLibrary::LoadResByPath(const FString& Path)
{
	FSoftObjectPath SoftPath(Path);
	UObject* Obj = SoftPath.TryLoad();

	return Obj;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RoleAssetBase.h"


FPrimaryAssetId URoleAssetBase::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(RoleType, GetFName());
}

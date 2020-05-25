// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerControllerBase.h"

void ARPGPlayerControllerBase::BeginPlay()
{
    // 加载所有道具
    LoadInventory();

    Super::BeginPlay();
}

bool ARPGPlayerControllerBase::LoadInventory()
{
    return true;
}

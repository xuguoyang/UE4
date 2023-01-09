// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Roles/PlayerCharacterBase.h"

APlayerCharacterBase::APlayerCharacterBase()
{

}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
}

void APlayerCharacterBase::UnPossessed()
{

}

void APlayerCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

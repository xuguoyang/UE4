// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Roles/RPGCharacterBase.h"
#include "PlayerCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API APlayerCharacterBase : public ARPGCharacterBase
{
	GENERATED_BODY()
	
public:
    APlayerCharacterBase();
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly)
	class UGridContainerComponent* GridContainerComponent;
};

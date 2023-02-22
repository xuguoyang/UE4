// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define/RPGItemTypes.h"
#include "Serialization/JsonWriter.h"
#include <Serialization/JsonReader.h>
#include <Serialization/JsonSerializer.h>
//#include "JsonObjectConverter.h"

#include "BaseItem.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API UBaseItem : public UObject
{
	GENERATED_BODY()
	
public:
    static UBaseItem* CreateItemFromNetInfo(const FItemNetInfo& newItemInfo, UObject* Owner);

	FItemNetInfo GetItemNetInfo();

	FString SerializeItemNetInfo();

	virtual void DoSerialize(TSharedRef<TJsonWriter<>> JsonWriter);

	virtual void DoDeserialize(TSharedPtr<FJsonObject> JsonObject);

	void InitFromNetInfo(const FItemNetInfo& theInfo);

public:
	virtual bool SetAmount(int32 AmountParam, EItemChangeReason ItemChangeReason, bool bShowNotification = false);

	virtual FText GetDescription();

	int32 GetItemId();

	FText GetItemName();

	bool CanMerge(UBaseItem* TargetItem);

	virtual bool CanMove();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	int32 MaxAmount;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "General")
	EItemChangeReason AddItemReason;

	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "NetInfo")
	FGuid UniqueId;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "NetInfo")
	int32 SlotIndex;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "NetInfo")
	int32 Amount;

	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<class UBaseContainer> OwnerContainer;
};

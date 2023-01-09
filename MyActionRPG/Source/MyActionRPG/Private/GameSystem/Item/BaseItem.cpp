// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Item/BaseItem.h"
#include "GameSystem/Item/BaseContainer.h"

UBaseItem* UBaseItem::CreateItemFromNetInfo(const FItemNetInfo& newItemInfo, UObject* Owner)
{
    return nullptr;
}

FItemNetInfo UBaseItem::GetItemNetInfo()
{
	FItemNetInfo ItemNetInfo;
	ItemNetInfo.UniqueId = UniqueId;
	ItemNetInfo.SlotIndex = SlotIndex;
	ItemNetInfo.ItemIndex = GetItemId();
	ItemNetInfo.Amount = Amount;
	ItemNetInfo.JsonStr = SerializeItemNetInfo();
	return ItemNetInfo;
}

FString UBaseItem::SerializeItemNetInfo()
{
	FString JsonStr = TEXT("");
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	DoSerialize(JsonWriter);
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	return JsonStr;
}

void UBaseItem::DoSerialize(TSharedRef<TJsonWriter<>> JsonWriter)
{

}

void UBaseItem::DoDeserialize(TSharedPtr<FJsonObject> JsonObject)
{

}

void UBaseItem::InitFromNetInfo(const FItemNetInfo& theInfo)
{
	UniqueId = theInfo.UniqueId;
	SlotIndex = theInfo.SlotIndex;
	Amount = theInfo.Amount;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(theInfo.JsonStr);
	//if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//DoDeserialize(JsonObject);
	}
}

bool UBaseItem::SetAmount(int32 AmountParam, EItemChangeReason ItemChangeReason, bool bShowNotification /*= false*/)
{
	if (OwnerContainer.IsValid())
	{
		if (!OwnerContainer.Get()->Items.Contains(this))
		{
			return false;
		}
	}

	if (Amount == AmountParam)
	{
		return false;
	}

	bool Addition = Amount < AmountParam;
	int32 AmountDetal = Addition ? AmountParam - Amount : Amount - AmountParam;

	Amount = FMath::Clamp(AmountParam, 0, MaxAmount);

	if (OwnerContainer.IsValid())
	{
		if (Amount == 0)
		{
			//É¾³ý
			OwnerContainer.Get()->RemoveItemByUniqueId(UniqueId, ItemChangeReason, -1, bShowNotification);
		}
		else
		{
			//Òì¶¯
			OwnerContainer.Get()->NoticeUpdateItem(this, ENotifyItemType::NOTIFY_ITEM_SETAMOUNT, bShowNotification);
		}
	}

	return true;
}

FText UBaseItem::GetDescription()
{
    return Description;
}

int32 UBaseItem::GetItemId()
{
	/*UMUGameData* GameData = UMUGameData::GetGameData();
	if (GameData)
	{
		return GameData->FindItemIndex(GetClass());
	}
	else*/
	{
		return -1;
	}
}

FText UBaseItem::GetItemName()
{
    return ItemName;
}

bool UBaseItem::CanMerge(UBaseItem* TargetItem)
{
	if (nullptr == TargetItem)
	{
		return false;
	}

	if (TargetItem == this)
	{
		return false;
	}

	if (GetClass() != TargetItem->GetClass())
	{
		return false;
	}

	if (Amount >= MaxAmount)
	{
		return false;
	}

	if (TargetItem->Amount > MaxAmount)
	{
		return false;
	}

	return true;
}

bool UBaseItem::CanMove()
{
    return true;
}

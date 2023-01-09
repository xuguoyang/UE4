// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyActionRPG.h"
#include <Engine/EngineTypes.h>
#include "RPGItemTypes.generated.h"

// ��������
UENUM(BlueprintType)
enum class EItemType : uint8
{
    ItemType_Equip,
    ItemType_Task,
};

// װ������
UENUM(BlueprintType)
enum class EItemEquipType : uint8
{
    ItemEquipType_None,
    ItemEquipType_Head,         // ͷ��װ��
    ItemEquipType_Body,         // �·�װ��
    ItemEquipType_Hand,         // �ֲ�װ��
    ItemEquipType_Weapon,       // ����װ��
    ItemEquipType_Leg,          // �Ȳ�װ��
    ItemEquipType_Foot,         // �Ų�װ��
};

// ���������
UENUM(BlueprintType)
enum class EShortcutType: uint8
{
    ShortcutGirdType_None,
    ShortcutGirdType_Item,
    ShortcutGirdType_Skill,
    ShortcutGirdType_WeaponSkill
};

USTRUCT(Blueprintable)
struct FItemNetInfo
{
	GENERATED_USTRUCT_BODY()

	// ΨһID ȫ
	UPROPERTY(VisibleAnywhere)
	FGuid UniqueId;

	// �����ڱ���е�Index ȫ
	UPROPERTY(VisibleAnywhere)
	int32 ItemIndex;

	// �ڵڼ��������� ȫ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SlotIndex;

	// ���ߵ�����ȫ
	UPROPERTY(VisibleAnywhere)
	int32 Amount;

	// ���ߵ���Ϣ ȫ
	UPROPERTY(VisibleAnywhere)
	FString JsonStr;

	FItemNetInfo()
	{
		UniqueId = FGuid();
		ItemIndex = -1;
		SlotIndex = -1;
		Amount = -1;
		JsonStr = TEXT("");
	}
};

// �����½�����ԭ��
// ���ֻ����������
UENUM()
enum class EItemChangeReason : uint8
{
	InvalidReason,
	// �̵�
	Shop,
	// ����
	Quest,
	// GM
	GM,
	// �ʼ�����
	MailAttachment,
	// �������
	DropItem,
	// ���
	SplitItem,
	// �ϲ�
	MergeItem,
	// ʹ��
	Use,
	// ���
	ClearAll,
	//����
	Exchange,
};

UENUM(BlueprintType)
enum class EContainerType : uint8
{
	CONTAINER_TYPE_BAG UMETA(DisplayName = "Bag"),
	CONTAINER_TYPE_EQUIPMENT UMETA(DisplayName = "Equipment"),
	CONTAINER_TYPE_SHORTCUT UMETA(DisplayName = "ShortCut"),
	CONTAINER_TYPE_APPEARANCE UMETA(DisplayName = "Appearance"),
	CONTAINER_TYPE_ALL
};

UENUM(BlueprintType)
enum class ENotifyItemType : uint8
{
	NOTIFY_NONE,
	NOTIFY_ITEM_UPDATENETINFO,
	NOTIFY_ITEM_ADD,
	NOTIFY_ITEM_REMOVE,
	NOTIFY_ITEM_EXCHANGE,
	NOTIFY_ITEM_SETAMOUNT,
	NOTIFY_INVENTORY_CLEARALL,
};

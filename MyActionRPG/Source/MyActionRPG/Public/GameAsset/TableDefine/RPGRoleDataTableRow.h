#pragma once

#include "MyActionRPG.h"
#include <Engine/DataTable.h>
#include <GameFramework/Actor.h>
#include <Styling/SlateBrush.h>
#include "RPGRoleDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct FRPGRoleDataTableRow: public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FRPGRoleDataTableRow() {}

    /**  ��ɫ�� */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    FText Name;

    /**  ������ʾ��icon */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    FSoftObjectPath Icon;

    /**  ���ö�Ӧ��actor */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    TSubclassOf<ARPGCharacterBase> RoleActor;
};
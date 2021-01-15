#pragma once

#include "MyActionRPG.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "SlateBrush.h"
#include "RPGRoleDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct FRPGRoleDataTableRow: public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FRPGRoleDataTableRow() {}

    /**  角色名 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    FText Name;

    /**  用来显示的icon */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    FSoftObjectPath Icon;

    /**  配置对应的actor */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    TSubclassOf<AActor> RoleActor;
};
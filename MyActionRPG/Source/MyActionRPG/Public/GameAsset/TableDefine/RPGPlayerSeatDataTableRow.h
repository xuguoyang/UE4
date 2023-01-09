#pragma once

#include "MyActionRPG.h"
#include <Engine/DataTable.h>
#include <Chaos/Vector.h>
#include <GameFramework/Actor.h>
#include "RPGPlayerSeatDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct FRPGPlayerSeatDataTableRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FRPGPlayerSeatDataTableRow() {}

    /** 玩家位置序号*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSeat)
    uint8 SeatIndex;

    /** 玩家当前控制角色在玩法中的初始位置*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSeat)
    FVector SpawnPosition;

    /** 玩家当前控制角色在玩法中的初始朝向*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSeat)
    FRotator SpawnRotation;
};
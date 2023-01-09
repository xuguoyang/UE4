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

    /** ���λ�����*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSeat)
    uint8 SeatIndex;

    /** ��ҵ�ǰ���ƽ�ɫ���淨�еĳ�ʼλ��*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSeat)
    FVector SpawnPosition;

    /** ��ҵ�ǰ���ƽ�ɫ���淨�еĳ�ʼ����*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSeat)
    FRotator SpawnRotation;
};
#pragma once

#include "GameAsset/TableDataAsset/TableAssetBase.h"
#include "TableAssetCommon.generated.h"

class UDataTable;

UCLASS(BlueprintType)
class MYACTIONRPG_API UTableAssetCommon : public UTableAssetBase
{
    GENERATED_BODY()
public:
    UTableAssetCommon();

    static UTableAssetCommon* pInstance;
    static UTableAssetCommon* Get()
    {
        if (pInstance == nullptr)
        {
            TArray<UObject*> Objects;
            if (URPGAssetManager::Get().GetPrimaryAssetObjectList(URPGAssetManager::CommonDataTableType, Objects))
            {
                pInstance = Cast<UTableAssetCommon>(Objects[0]);
            }    
        }
        
        return pInstance;
    }

    /** 角色列表*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= TableAssetCommon)
    UDataTable* TableRoleList;

    /** 玩家位置列表*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TableAssetCommon)
    UDataTable* TablePlayerList;

    /** 道具配置表*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TableAssetCommon)
    UDataTable* TableItem;
};
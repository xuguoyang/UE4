#pragma once

#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "RPGAssetManager.h"
#include "TableAssetBase.generated.h"


UCLASS(Abstract, BlueprintType)
class MYACTIONRPG_API UTableAssetBase : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    UTableAssetBase(){}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataAsset)
    FPrimaryAssetType	AssetType;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;

};
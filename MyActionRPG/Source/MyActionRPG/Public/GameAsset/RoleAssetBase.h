#pragma once

#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "RPGAssetManager.h"
#include "RoleAssetBase.generated.h"

UCLASS(Abstract, BlueprintType)
class MYACTIONRPG_API URoleAssetBase : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    URoleAssetBase(){}

    /** 角色类型 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Role)
    FPrimaryAssetType	RoleType;

    /**  角色名 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    FText Name;

    /**  用来显示的icon */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    FSlateBrush Icon;

    /**  配置对应的actor */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Role)
    TSubclassOf<AActor> RoleActor;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
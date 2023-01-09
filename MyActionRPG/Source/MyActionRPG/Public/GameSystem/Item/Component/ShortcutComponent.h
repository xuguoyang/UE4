// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShortcutComponent.generated.h"

// ���������
USTRUCT(BlueprintType)
struct FShortcutGird
{
    GENERATED_USTRUCT_BODY()

        FShortcutGird() :
        Index(-1),
        Item(nullptr)
    {}

    // ��������
    UPROPERTY()
    uint32 Index;

    // ��ӵĶ���
    UPROPERTY()
    UObject* Item;

    // �����������
    UPROPERTY(EditDefaultsOnly, Category = Shortcut)
    EShortcutType Type;

    // ʹ����Ʒ
    bool UseItem();

    // ��ӵ���
    void Add(UObject* InItem);

    // ���
    void Clear();

    // �Ƿ�������
    bool CanAdd(UObject* InItem);

    /** ����֧������һ��Ĭ�ϵļ���*/
    UPROPERTY(EditDefaultsOnly, Category = Shortcut)
    TSubclassOf<UGameplayAbility> Default;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONRPG_API UShortcutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShortcutComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
    /** ��ӵ������*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    bool AddShortcutItem(UObject* Item, int32 Index = -1);

    /** �ӿ�������Ƴ�*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    bool RemoveShortcutItem(int32 Index);

    /** ʹ��ĳ��������еĶ���*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    bool ShortcutUse(int32 Index);

    /** ���ĳ�����������*/
    UFUNCTION(BlueprintCallable, Category = Shortcut)
    EShortcutType GetShortcutType(UObject* Item);

// ��ͻ��˷���Ϣ
protected:
    UFUNCTION(Reliable, Client, BlueprintCallable)
    void ClientAddSuccessed(int32 Index);

    UFUNCTION(Reliable, Client, BlueprintCallable)
    void ClientRemoveSuccessed(int32 Index);

// �����˷���Ϣ
protected:
    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
    void ServerAddShortcutItem(int32 Index, UObject* Item);

    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
    void ServerRemoveShortcutItem(int32 Index);

    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation)
    void ServerShortcutUse(int32 Index);

private:
    /** �����Ƿ���Ч*/
    bool IsIndexVaild(int32 Index);

    /** �Ƿ������ӵ������*/
    bool CanAdd(int32 Index, UObject* Item);

    /** �Ƿ�����Ƴ��������*/
    bool CanRemove(int32 Index);

    /** �Ƿ����ʹ��*/ 
    bool CanUse(int32 Index);

    /** ��ȡһ���յĸ���*/
    int32 GetEmptyGird(EShortcutType Type);

    /** �������ó�ʼ����Ӽ���*/
    void InitAbilityAdd();
protected:
    // ���п��������,����ͼ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Default)
    TArray<FShortcutGird> ShortcutGirds;
};

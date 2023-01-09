// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>


class GameStageBase;
 
namespace EGameStageBase
{
    enum stage: uint8
    {
        EStage_None,
        EStage_Login,
        EStage_Select,
        EStage_Playing,
    };
};

class GameStageManage
{
public:
    GameStageManage();

    static GameStageManage& GetInstance();
public:
    // 切换当前游戏阶段
    void SwitchStage(EGameStageBase::stage eStage);

    // level加载完成
    void LoadLevelComplete(const float LoadTime, const FString& MapName);
private:
    void InitStages();

    TSharedPtr<GameStageBase> GetGameStage(EGameStageBase::stage eStage);
private:
    // 当前阶段
    EGameStageBase::stage m_eStage;

    // 所有阶段列表
    typedef std::map<EGameStageBase::stage, TSharedPtr<GameStageBase>> GameStageMap;
    GameStageMap m_mapGameStage;

    // 当前阶段的level名称
    FString m_sLevelName;
};
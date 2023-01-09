#include "GamePlaying/GameClientStage/GameStageManager.h"
#include "GamePlaying/GameClientStage/GameStageLogin.h"
#include "GamePlaying/GameClientStage/GameStageSelect.h"
#include "GamePlaying/GameClientStage/GameStagePlaying.h"

GameStageManage::GameStageManage():
    m_eStage(EGameStageBase::EStage_None)
{
    InitStages();
}

GameStageManage& GameStageManage::GetInstance()
{
    static GameStageManage Instance;
    return Instance;
}

void GameStageManage::InitStages()
{
    m_mapGameStage.clear();

    m_mapGameStage.insert(std::make_pair(EGameStageBase::EStage_Login, MakeShareable(new GameStageLogin())));
    m_mapGameStage.insert(std::make_pair(EGameStageBase::EStage_Select, MakeShareable(new GameStageSelect())));
    m_mapGameStage.insert(std::make_pair(EGameStageBase::EStage_Playing, MakeShareable(new GameStagePlaying())));
}

TSharedPtr<GameStageBase> GameStageManage::GetGameStage(EGameStageBase::stage eStage)
{
    GameStageMap::iterator iterFind = m_mapGameStage.find(eStage);
    if (iterFind == m_mapGameStage.end())
    {
        return NULL;
    }

    return iterFind->second;
}

void GameStageManage::SwitchStage(EGameStageBase::stage eStage)
{
    // �뿪��ǰ״̬
    TSharedPtr<GameStageBase> GameStagePtr = GetGameStage(m_eStage);
    if (GameStagePtr.IsValid())
    {
        GameStagePtr->OnLeave();
    }

    // ����Ŀ��״̬
    GameStagePtr = GetGameStage(eStage);
    if (GameStagePtr.IsValid())
    {
        GameStagePtr->OnEntry();
        m_eStage = eStage;
    }
}

void GameStageManage::LoadLevelComplete(const float LoadTime, const FString& MapName)
{
    if (m_eStage == EGameStageBase::EStage_None)        // ��ʼ״̬
    {
        SwitchStage(EGameStageBase::EStage_Login);
    }
}


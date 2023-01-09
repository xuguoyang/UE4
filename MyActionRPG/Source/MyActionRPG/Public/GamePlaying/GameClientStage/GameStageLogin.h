#pragma once
#include "GamePlaying/GameClientStage/GameStageBase.h"

class GameStageLogin: public GameStageBase
{
public:
    GameStageLogin();

public:
    virtual void OnEntry() override;
    virtual void OnLeave() override;
};
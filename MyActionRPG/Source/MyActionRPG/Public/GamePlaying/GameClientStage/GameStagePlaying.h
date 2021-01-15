#pragma once

#include "GameStageBase.h"

class GameStagePlaying : public GameStageBase
{
public:
    GameStagePlaying();

public:
    virtual void OnEntry() override;
    virtual void OnLeave() override;
};
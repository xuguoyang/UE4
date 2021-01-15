#pragma once

#include "GameStageBase.h"

class GameStageSelect : public GameStageBase
{
public:
    GameStageSelect();

public:
    virtual void OnEntry() override;
    virtual void OnLeave() override;
};
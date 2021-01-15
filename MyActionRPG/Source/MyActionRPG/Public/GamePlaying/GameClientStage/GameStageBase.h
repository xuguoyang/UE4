#pragma once

class GameStageBase
{
public:
    virtual void OnEntry() = 0;
    virtual void OnLeave() = 0;
};
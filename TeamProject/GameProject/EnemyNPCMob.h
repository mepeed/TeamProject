#pragma once

#include "Character.h"

namespace SSB
{
    class EnemyNPCMob : public Character
    {
    public:
        float m_BattleRange = 5;
        float m_SpotRange = 30;

    public:
        float GetBattleRange();
        float GetSpotRange();
    };
}

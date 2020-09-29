/*
 * Made by ConanHUN for Single Player Project - Ashamane (legion) repack
 */

#include "Config.h"
#include "ScriptPCH.h"

class PandarenFaction : public PlayerScript
{
public:
    PandarenFaction() : PlayerScript("PandarenFaction") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (sConfigMgr->GetBoolDefault("Skip.Pandaren.Enable", true))
        {
            if (player->getRace() == RACE_PANDAREN_NEUTRAL)
            {
                player->ShowNeutralPlayerFactionSelectUI();
            }
        }
    }
};

void AddSC_PandarenFaction()
{
    new PandarenFaction;
}
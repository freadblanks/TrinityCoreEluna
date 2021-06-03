/*
 * Made by ConanHUN for Single Player Project - Ashamane (legion) repack
 */

#include "Config.h"
#include "ScriptPCH.h"

#ifndef SC_PRECOMPILED_H
#define SC_PRECOMPILED_H

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "SpellScript.h"

#include "SpellAuraEffects.h"
#include "AccountMgr.h"
#include "ArenaTeamMgr.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "Language.h"
#include "MovementGenerator.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "SpellAuras.h"
#include "TargetedMovementGenerator.h"
#include "WeatherMgr.h"
#include "Player.h"
#include "Pet.h"
#include "LFG.h"
#include "GroupMgr.h"
#include "MMapFactory.h"
#include "DisableMgr.h"
#include "SpellHistory.h"
#include "MiscPackets.h"
#include "Transport.h"
#include "DatabaseEnv.h"
#include "World.h"
#include "WorldSession.h"



#ifdef _WIN32
#include <windows.h>
#endif

#endif

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

class Player_skybox : public PlayerScript //Dis no respect sneaky case
{
public:
    Player_skybox() : PlayerScript("Player_skybox") {}
    void OnLogin(Player* player, bool firstLogin)
    {

        //Select Customs
        QueryResult getDisplay = WorldDatabase.PQuery("SELECT skybox from player_skybox WHERE guid = %u", player->GetGUID().GetCounter());
        if (!getDisplay)
            return;

        Field* field = getDisplay->Fetch();

        uint32 skyboxId = field[0].GetUInt32();
        uint32 map = player->GetMapId();
        uint32 mapCache = player->GetMapId();

        if (skyboxId != 0)
        {
            if (map > 0)
            {

                if (player->GetMapId() >= 0)
                {
                    if (MapEntry const* entry = sMapStore.AssertEntry(map))
                        map = entry->ParentMapID;
                }

                uint32 lightId = DB2Manager::GetMapLightId(map);

                WorldPacket data(SMSG_OVERRIDE_LIGHT, 12);
                data << lightId;
                data << skyboxId;
                data << 200;

                sWorld->SendMapMessage(mapCache, WorldPackets::Misc::OverrideLight(int32(lightId), int32(200), int32(skyboxId)).Write());

            }
            else
            {
                uint32 lightId = DB2Manager::GetMapLightId(map);

                WorldPacket data(SMSG_OVERRIDE_LIGHT, 12);
                data << lightId;
                data << skyboxId;
                data << 200;

                player->GetSession()->SendPacket(&data, true);
            }

        }
    }

};

void AddSC_PandarenFaction()
{
    new PandarenFaction;
    new Player_skybox();
}

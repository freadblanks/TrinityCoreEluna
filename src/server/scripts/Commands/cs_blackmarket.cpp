#include "BlackMarketMgr.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "RBAC.h"
#include "WorldSession.h"


#include "Bag.h"
#include "BattlefieldMgr.h"
#include "BattlegroundMgr.h"
#include "CellImpl.h"
#include "ChannelPackets.h"
#include "ChatPackets.h"
#include "Conversation.h"
#include "DB2Stores.h"
#include "GameTime.h"
#include "GossipDef.h"
#include "GridNotifiersImpl.h"
#include "InstanceScript.h"
#include "M2Stores.h"
#include "MapManager.h"
#include "MovementPackets.h"
#include "ObjectAccessor.h"
#include "PhasingHandler.h"
#include "PoolMgr.h"
#include "QuestPools.h"
#include "SpellMgr.h"
#include "SpellPackets.h"
#include "Transport.h"
#include "World.h"
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <sstream>

class blackmarket_commandscript : public CommandScript
{
public:
    blackmarket_commandscript() : CommandScript("blackmarket_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> blackmarketCommandTable =
        {
            { "setduration",    rbac::RBAC_PERM_COMMAND_BLACKMARKET_SET_DURATION,   true,  &HandleBMSetAuctionDurationCommand,                  "" },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "blackmarket",    rbac::RBAC_PERM_COMMAND_BLACKMARKET,                false, nullptr,                    "", blackmarketCommandTable },
        };
        return commandTable;
    }

    static bool HandleBMSetAuctionDurationCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* marketId1 = strtok((char*)args, " ");

        if (!marketId1)
            return false;

        char* durationSeconds1 = strtok(nullptr, " ");

        if (!durationSeconds1)
            return false;

        uint32 marketId = atoi(marketId1);
        uint32 durationSeconds = atoi(durationSeconds1);

        if (BlackMarketEntry* entry = sBlackMarketMgr->GetAuctionByID(marketId))
            entry->SetStartTime(durationSeconds + time(nullptr) - sBlackMarketMgr->GetTemplateByID(entry->GetMarketId())->Duration);

        return true;
    }
};

void AddSC_blackmarket_commandscript()
{
    new blackmarket_commandscript();
}

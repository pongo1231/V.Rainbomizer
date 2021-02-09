#include "missions_Order.hh"
#include "mission/missions_Globals.hh"
#include "missions.hh"

#include "rage.hh"

#include <cstdint>
#include <map>
#include <string.h>
#include <vector>
#include <random>

#ifdef ENABLE_DEBUG_SERVER
#include "debug/actions.hh"
#endif

using MR = MissionRandomizer_Components;

void
MissionRandomizer_OrderManager::Process (scrThreadContext *ctx,
                                         scrProgram *      program)
{
    if (program->m_nScriptHash == "standard_global_init"_joaat
        && ctx->m_nState == eScriptState::KILLED)
        {
            uint32_t seed = static_cast<uint32_t> (
                std::hash<std::string>{}(MR::Config ().Seed));

            InitialiseMissionsMap (seed);
            Update_gMissions ();

            bInitialised = true;
        }

    if (program->m_nScriptHash == "flow_controller"_joaat && ctx->m_nIp == 0)
        {
            RemoveMissionFlowHeistBoards ();
        }

#ifdef ENABLE_DEBUG_SERVER
    if (ActionsDebugInterface::sm_ReloadMissionsRequested)
        {
            uint32_t seed = static_cast<uint32_t> (
                std::hash<std::string>{}(MR::Config ().Seed));

            InitialiseMissionsMap (seed);
            Update_gMissions ();
            ActionsDebugInterface::sm_ReloadMissionsRequested = false;
        }
#endif
}

void
MissionRandomizer_OrderManager::InitialiseMissionsMap (unsigned int seed)
{
    std::vector<uint32_t> missions;
    std::vector<uint32_t> order;
    std::mt19937          engine{seed};

    for (const auto &s : MR::sm_Data.GetMissionDataMap ())
        missions.push_back (s.first);

    order = missions;
    std::shuffle (order.begin (), order.end (), engine);

    uint32_t forcedHash = -1u;
    if (MR::Config ().ForcedMission.size ())
        forcedHash = rage::atStringHash (MR::Config ().ForcedMission);

    for (unsigned int i = 0; i < missions.size (); i++)
        {
            if (forcedHash == -1u)
                m_MissionsMap[missions[i]] = order[i];
            else
                m_MissionsMap[missions[i]] = forcedHash;
        }

    m_Choices.AgencyFiretruck = engine () % 2;
    m_Choices.DocksBlowUpBoat = engine () % 2;
    m_Choices.FinaleHeli      = engine () % 2;
    m_Choices.JewelStealth    = engine () % 2;
}

void
MissionRandomizer_OrderManager::Update_gMissions ()
{
    if (m_MissionInfos.empty ())
        {
            for (unsigned int i = 0; i < MR::sm_Globals.g_Missions.nSize; i++)
                {
                    auto &data = MR::sm_Globals.g_Missions.Data[i];
                    if (!MR::sm_Data.IsValidMission (data.nThreadHash))
                        continue;

                    m_MissionInfos[data.nThreadHash]
                        = {data.nThreadHash, i, &data, data,
                           MR::sm_Data.GetMissionData (data.nThreadHash)};
                }
        }

    for (unsigned int i = 0; i < MR::sm_Globals.g_Missions.nSize; i++)
        {
            auto &origMission = MR::sm_Globals.g_Missions.Data[i];

            if (!m_MissionsMap.count (origMission.nThreadHash))
                continue;

            uint32_t newHash    = m_MissionsMap[origMission.nThreadHash];
            auto &   newMission = GetMissionInfo (newHash)->DefCopy;

            strncpy (origMission.sMissionThread, newMission.sMissionThread, 24);
            origMission.nThreadHash = newMission.nThreadHash;

            origMission.BITS_MissionFlags.NO_STAT_WATCHER
                = newMission.BITS_MissionFlags.NO_STAT_WATCHER;
            origMission.BITS_MissionFlags.DISABLE_MISSION_SKIPS
                = newMission.BITS_MissionFlags.DISABLE_MISSION_SKIPS;
        }
}

void
MissionRandomizer_OrderManager::RemoveMissionFlowHeistBoards ()
{
    auto NopFlowCommands = [] (unsigned int start, unsigned int end) {
        for (unsigned int i = start; i <= end; ++i)
            {
                // Just set it to a non-existant flow command, it'll skip it.
                if (auto cmds = MR::sm_Globals.g_MissionFlowCommands.Get ())
                    cmds->Data[i].CommandHash = "123robot"_joaat;
            }
    };

    // Agency Boards
    NopFlowCommands(34, 34);
    NopFlowCommands(36, 49);

    // Docks Board
    NopFlowCommands(363, 363);
    NopFlowCommands(369, 378);

    // Finale Heist Board (+ finale_heist_intro2)
    NopFlowCommands(1017, 1033);

    // Jewel Heist Board
    NopFlowCommands(1206, 1220);

    // Rural Bank Heist Board
    NopFlowCommands(1577, 1579);
    NopFlowCommands(1585, 1586);
}

void
MissionRandomizer_OrderManager::RestoreOriginalMissionInfo (uint32_t missionId)
{
    auto info = GetMissionInfoFromId (missionId);
    if (!info || !info->pDef)
        return;

    m_StoredRandomInfo                        = *info->pDef;
    MR::sm_Globals.g_Missions.Data[missionId] = info->DefCopy;
}

void
MissionRandomizer_OrderManager::ReapplyRandomMissionInfo (uint32_t missionId)
{
    MR::sm_Globals.g_Missions.Data[missionId] = m_StoredRandomInfo;
}

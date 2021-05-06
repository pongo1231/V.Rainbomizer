#pragma once

#include <Utils.hh>
#include <array>

#include "CModelInfo.hh"
#include "peds_Compatibility.hh"

#include <CPed.hh>
#include <CutSceneManager.hh>
#include <CModelIndices.hh>
#include <rage.hh>
#include <utility>

struct fwRefAwareBase;

class PedRandomizer_PlayerFixes
{
    inline static bool                           sm_Initialised = false;
    inline static std::array<CModelIndices *, 3> sm_PlayerIdxPtr;

    /*******************************************************/
    static int
    GetRandomSpecialAbility ()
    {
        static unsigned int numSpecialAbilities = 0;
        if (numSpecialAbilities == 0)
            {
                static struct parEnumTranslationMap
                {
                    uint32_t hash;
                    int32_t  value;
                } *enumTranslationTable
                    = hook::get_pattern<parEnumTranslationMap> (
                        "c0 24 e0 31 ff ff ff ff 64 d0 6a 7f 00 00 00 00");

                while (enumTranslationTable[++numSpecialAbilities].hash != 0)
                    ;
            }

        return RandomInt (numSpecialAbilities - 2);
    }

    /*******************************************************/
    template <auto &CutSceneManager_InitPlayerObj>
    static void
    CorrectPlayerObjIdx (CutSceneManager *mgr)
    {
        CPed *          player = CPedFactory::Get ()->pPlayer;
        CBaseModelInfo *model  = PedRandomizerCompatibility::GetOriginalModel (
            CPedFactory::Get ()->pPlayer);

        // Restore original model for this function and change it back after
        // done.
        std::swap (player->m_pModelInfo, model);
        CutSceneManager_InitPlayerObj (mgr);
        std::swap (player->m_pModelInfo, model);
    }

    /*******************************************************/
    static CModelIndices *
    GetPlayerModelIndexPtr (uint32_t hash)
    {
        switch (hash)
            {
            case "player_zero"_joaat: return sm_PlayerIdxPtr[0];
            case "player_one"_joaat: return sm_PlayerIdxPtr[1];
            case "player_two"_joaat: return sm_PlayerIdxPtr[2];
            }

        return nullptr;
    }

    /*******************************************************/
    static void
    InitialisePlayerIndicesPointers ()
    {
        static bool bPlayerIndicesIntiailised = false;
        if (std::exchange (bPlayerIndicesIntiailised, true))
            return;

        for (auto index = CModelIndices::GetHead (); index;
             index      = index->pNext)
            {
                switch (index->nHash)
                    {
                    case "player_zero"_joaat: sm_PlayerIdxPtr[0] = index; break;
                    case "player_one"_joaat: sm_PlayerIdxPtr[1] = index; break;
                    case "player_two"_joaat: sm_PlayerIdxPtr[2] = index; break;
                    }
            }
    }

    /*******************************************************/
    static void
    ResetPlayerHashChanges ()
    {
        sm_PlayerIdxPtr[0]->nHash = "player_zero"_joaat;
        sm_PlayerIdxPtr[1]->nHash = "player_one"_joaat;
        sm_PlayerIdxPtr[2]->nHash = "player_two"_joaat;
    }

    /*******************************************************/
    template <auto &fwRefAwareBase__AddKnownRef>
    static void
    UpdatePlayerHash_Trampoline (fwRefAwareBase *base, void *ref)
    {
        fwRefAwareBase__AddKnownRef (base, ref);
        UpdatePlayerHash ();
    }

public:
    /*******************************************************/
    static void
    Initialise ()
    {
        if (sm_Initialised)
            return;

        // Hook for cutscenes to properly get the object model to register for
        // the cutscene.
        REGISTER_HOOK ("8b d9 40 38 3d ? ? ? ? 75 ? e8", 11,
                       CorrectPlayerObjIdx, void, CutSceneManager *);

        // This patch enables phone models for all ped models
        injector::MakeNOP (
            hook::get_pattern (
                "8b ? ? ? ? ? 44 38 70 09 74 ? 40 84 f6 75 ? b9 30 00 00 00 ",
                10),
            2);

        sm_Initialised = true;

        // To make UpdatePlayerHash also be called after a call to
        // CHANGE_PLAYER_PED (and several other functions). Can also be extended
        // to add CPedFactory::CreatePlayer, but I didn't see the need.
        // UpdatePlayerHash is also called by Ped Randomizer to catch those
        // cases.
        REGISTER_HOOK (
            "? 8d ? 08 ? 8b cf ? 89 ? ? e8 ? ? ? ? ? 8d 0d ? ? ? ? ? 8b d7", 11,
            UpdatePlayerHash_Trampoline, void, fwRefAwareBase *, void *);
    }

    /*******************************************************/
    static void
    RandomizeSpecialAbility (CPed *ped)
    {
        auto model = static_cast<CPedModelInfo *> (ped->m_pModelInfo);
        model->GetInitInfo ().m_nSpecialAbility = GetRandomSpecialAbility ();
    }

    /*******************************************************/
    static void
    UpdatePlayerHash ()
    {
        InitialisePlayerIndicesPointers ();
        ResetPlayerHashChanges ();

        auto *player = CPedFactory::Get ()->pPlayer;
        if (!player)
            return;

        auto *model = PedRandomizerCompatibility::GetOriginalModel (player);
        if (!model)
            return;

        GetPlayerModelIndexPtr (model->m_nHash)->nHash
            = player->m_pModelInfo->m_nHash;
    }
};

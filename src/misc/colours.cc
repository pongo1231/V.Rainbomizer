#include <CHud.hh>
#include <Utils.hh>
#include "common/config.hh"
#include "HSL.hh"
#include <CVehicle.hh>
#include <map>
#include <stdexcept>

void (*CHud__SetHudColour) (int, int, int, int, int);
uint32_t (*CCustomShaderEffectVehicle_SetForVehicle_134) (
    CCustomShaderEffectVehicle *, CVehicle *);

class ColoursRandomizer
{
    struct VehicleColourData
    {
        CARGB Colours[4];
    };

    inline static std::map<CVehicle *, VehicleColourData> mColourData;

    /*******************************************************/
    static void
    SetNewHudColour (int index, int r, int g, int b, int a)
    {
        using Rainbomizer::HSL;

        HSL colour (CARGB (a, r, g, b));
        colour.h = RandomFloat (360);
        colour.s = RandomFloat (0.5, 1);
        colour.l = RandomFloat (std::max (0.0f, colour.l - 0.25f),
                                std::min (1.0f, colour.l + 0.25f));

        CARGB newColour = colour.ToARGB ();

        CHud__SetHudColour (index, newColour.r, newColour.g, newColour.b, a);
    }

    /*******************************************************/
    static void
    RestoreVehicleColourData (CCustomShaderEffectVehicle *shader, CVehicle *veh)
    {
        try
            {
                auto & data    = mColourData.at (veh);
                CARGB *colours = shader->GetColours ();

                for (int i = 0; i < 4; i++)
                    colours[i] = data.Colours[i];
            }
        catch (...)
            {
            }
    }

    /*******************************************************/
    static bool
    StoreVehicleColourData (CCustomShaderEffectVehicle *shader, CVehicle *veh)
    {
        auto & data    = mColourData[veh];
        CARGB *colours = shader->GetColours ();

        bool changed = false;
        for (int i = 0; i < 4; i++)
            if (std::exchange (data.Colours[i], colours[i]) != colours[i])
                changed = true;

        return changed;
    }

    /*******************************************************/
    static uint32_t
    RandomizeVehicleColour (CCustomShaderEffectVehicle *shader, CVehicle *veh)
    {
        RestoreVehicleColourData (shader, veh);

        uint32_t ret
            = CCustomShaderEffectVehicle_SetForVehicle_134 (shader, veh);
        CARGB *colours = shader->GetColours ();

        if (StoreVehicleColourData (shader, veh))
            {
                for (int i = 0; i < 4; i++)
                    {
                        using Rainbomizer::HSL;

                        colours[i]
                            = HSL (RandomFloat (360), 1.0, RandomFloat (1.0))
                                  .ToARGB ();
                    }
            }

        return ret;
    }

public:
    /*******************************************************/
    ColoursRandomizer ()
    {
        bool RandomizeHudColours = true;
        bool RandomizeCarColours = true;

        if (!ConfigManager::ReadConfig (
                "ColourRandomizer",
                std::pair ("RandomizeHudColours", &RandomizeHudColours),
                std::pair ("RandomizeCarColours", &RandomizeCarColours)))
            return;

        InitialiseAllComponents ();

        // Hud Colours
        // ----------
        if (RandomizeHudColours)
            RegisterHook ("8b ? ? ? ? ? 8b ? ? ? ? ? 8b cb 89 44 ? ? e8", 18,
                          CHud__SetHudColour, SetNewHudColour);

        // Car Colours
        // ---------
        if (RandomizeCarColours)
            {
                void *addr = hook::get_pattern (
                    "85 c9 74 ? ? 8b d3 e8 ? ? ? ? 84 c0 74 ? ? 84 ff 74", 7);

                RegisterJmpHook<13> (
                    injector::GetBranchDestination (addr).get<void> (),
                    CCustomShaderEffectVehicle_SetForVehicle_134,
                    RandomizeVehicleColour);
                // RegisterHook (addr, RandomizeVehicleColour);
            }
    }
} _cols;

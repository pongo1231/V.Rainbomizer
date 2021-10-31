const char configDefault[] = R"(
########################################################

# Rainbomizer for V
# Copyright (C) 2020-2021 - Parik

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.	If not, see <https://www.gnu.org/licenses/>.
#
#######################################################

# General Configuration
# Refer to the readme to get the description of all the randomizers.
[Randomizers]

RainbomizerNews = true
RainbomizerCredits = true

TrafficRandomizer = true
DispatchRandomizer = true
ColourRandomizer = true
WeaponRandomizer = true
WeaponStatsRandomizer = true
CutSceneRandomizer = true
ScriptVehicleRandomizer = true
VoiceLineRandomizer = true
SfxRandomizer = true
TimecycleRandomizer = true
MissionRandomizer = true
PedRandomizer = true
HandlingRandomizer = false

#######################################################
[TimecycleRandomizer]

RandomizeWeather = true # Randomize the weather (properties like sun, rain, etc.)
RandomizeTimecycle = true # Randomize the appearance of the sky/ground.

#######################################################
# """"FILTER/LIGHTING""""" HATERS LOOK HERE (Spoiler Alert: There's no """FILTER""")
#######################################################
# These control how random the timecycles are gonna be.

# The first setting is how many times out of 100 the sky is gonna be randomised through presets.
# So this includes everything from just the skies having random moons all the way to crazy wacky super-dark pink sky
RandomizeTimecycleOdds = 25

# This second setting is how many times out of 100 (WHEN THE TIMECYCLE IS NOT RANDOM),
# the non-randomized timecycle uses a timesample from a different NORMAL timecycle.
# So if it's supposed to be clear and night time, it may be cloudy and day time.
RandomizeTimeOdds = 75

#######################################################
[ModCompatibility]

# If you don't want weapons and peds spawned by trainers/other mods to be random,
# disable this option. Vehicles spawned by mods are never randomized.
RandomizeMods = true

#######################################################
[TrafficRandomizer]

# Enable this if you're having frame drops because of big planes exploding.
DisableBigVehicles = false

EnablePlanes = true
EnableHelis = true
EnableBoats = true # Don't be a boat hater, keep this enabled :wink:

#######################################################
[ColourRandomizer]

RandomizeHudColours = true
RandomizeCarColours = true

#######################################################
[ScriptVehicleRandomizer]

LogSpawnedVehicles = false # Logs all the spawned script vehicles

#######################################################

TunableFile = "Timecyc/Default.txt"

RandomizeEveryFade = true

#######################################################
[VoiceLineRandomizer]

IncludeDLCLines = true # Includes DLC lines in randomization

#######################################################
[MissionRandomizer]

Seed = ""
ForceSeedOnSaves = false

ForcedMission = "" # All missions will start this instead of a random mission.

EnableFastSkips = false # Mission skips will be enabled after failing the mission once. Useful for testing

#######################################################
[PedRandomizer]

RandomizePlayer = true
RandomizePeds = true
RandomizeCutscenePeds = true # only if UseCutsceneModelsFile is enabled

RandomizeSpecialAbility = true
IncludeUnusedAbilities = false # sprint boost, aimbot and two abilities that don't work (+ any other R* may decide to add and not use in the future).

ForcedPed = ""
ForcedClipset = ""

EnableNSFWModels = false
UseCutsceneModelsFile = true

# Missions like Lamar Down, Big Score Heist and other major shootout missions disable
# new peds to be loaded, which causes a significant hit to the ped variety in the mission
# (normally resulting in too many player models). This makes it so that doesn't happen.
EnableNoLowBudget = true

# This option makes it so enemy blips are always visible. This helps when the enemies are
# randomized into a small animal that's difficult to locate
EnableBlipsAlwaysVisible = true

# Reduce the odds of the player models being considered to spawn
# (from 0 (no player models) to 100 (some player models))
OddsOfPlayerModels = 20
)";

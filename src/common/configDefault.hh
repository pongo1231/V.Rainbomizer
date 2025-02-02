const char configDefault[] = R"(
########################################################

# Rainbomizer for V
# Copyright (C) 2020-2022 - Parik

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

RainbomizerCredits = true

CarGeneratorRandomizer = true
ClothesRandomizer = true
TrafficRandomizer = true
DispatchRandomizer = true
HudRandomizer = true
VehicleAppearanceRandomizer = true
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
SwitchSceneRandomizer = false
RespawnRandomizer = true
LightRandomizer = true

#######################################################
[VoiceLineRandomizer]

# Whether to include DLC lines in randomization. Set to false if you don't want to hear
# DLC voice lines.
IncludeDLCLines = true

# Voice Line Randomizer has 4 states it can be in:
# It can either be:
#
# 1. Completely Random (voice lines have a completely random order)
# 2. Ordered Random (consecutive voice lines are from the same dialogue sequence)
# 3. Some Random (voice lines are normal except some random lines)
# 4. No Random (all voice lines are normal).
#
# Each of these 4 state is randomly chosen and stays for a duration which can be
# configured. This duration also decides how likely they are to be chosen. (i.e longer
# durations means more likely to be chosen).
#
# All durations are in seconds

TrulyRandomDuration = 15
OrderedRandomDuration = 45
SomeRandomDuration = 15
NoRandomDuration = 15

# Percentage of times the conversation will have random voice lines from the same character
# (For Example, Franklin voice lines will always be other Franklin voice lines)
# 100 means that all voice lines will be from the same character
# 0 means that all voice lines will be random
# Note: It doesn't apply to OrderedRandom state.
SameSpeakerPercentage = 75

# During Some Random state, this option decides how likely a voice line is to be randomized.
# If you set this to 100, it behaves the same as TrulyRandom.
# 0 is the same as NoRandom
PercentageRandomOnSomeRandom = 65

# How many seconds before the ordered dialogue is re-randomized to a different sequence.
# If you set this to 0, it behaves the same as TrulyRandom.
OrderedDialogueChangeFrequency = 20

#######################################################
[TimecycleRandomizer]

RandomizeWeather = true # Randomize the weather (properties like sun, rain, etc.)
RandomizeTimecycle = true # Randomize the appearance of the sky/ground.
RandomSnow = true # snowy LS

RainOdds = 5.0
ThunderstormOdds = 1.0
SnowOdds = 5.0 # snowy LS

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
[VehicleAppearanceRandomizer]

RandomizeCarColours = true
RandomizeCarUpgrades = true

# Value of 50 = 50% of cars will have random horn/armour
RandomizeLicensePlateOdds = 75
RandomizeHornOdds = 45
RandomizeArmourOdds = 45

#######################################################
[ScriptVehicleRandomizer]

LogSpawnedVehicles = false # Logs all the spawned script vehicles

#######################################################

TunableFile = "Timecyc/Default.txt"

RandomizeEveryFade = true

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

#######################################################
[LightRandomizer]

# Set to 0 if you want all lights to be same colours.
# Setting it to a high value means light colours change more slowly over distance.
LightShiftFrequency = 1.0

# 0-100, with 0 being lights are never random to 100 being lights are always random.
RandomizeOdds = 85.0)";

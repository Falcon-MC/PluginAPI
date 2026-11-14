#pragma once

#include "falcon/falcon_api.h"

#include <cstdint>
#include <string>

namespace falcon {
    enum class EffectType : uint32_t {
        Speed = FALCON_EFFECT_SPEED,
        Slowness = FALCON_EFFECT_SLOWNESS,
        Haste = FALCON_EFFECT_HASTE,
        MiningFatigue = FALCON_EFFECT_MINING_FATIGUE,
        Strength = FALCON_EFFECT_STRENGTH,
        InstantHealth = FALCON_EFFECT_INSTANT_HEALTH,
        InstantDamage = FALCON_EFFECT_INSTANT_DAMAGE,
        JumpBoost = FALCON_EFFECT_JUMP_BOOST,
        Nausea = FALCON_EFFECT_NAUSEA,
        Regeneration = FALCON_EFFECT_REGENERATION,
        Resistance = FALCON_EFFECT_RESISTANCE,
        FireResistance = FALCON_EFFECT_FIRE_RESISTANCE,
        WaterBreathing = FALCON_EFFECT_WATER_BREATHING,
        Invisibility = FALCON_EFFECT_INVISIBILITY,
        Blindness = FALCON_EFFECT_BLINDNESS,
        NightVision = FALCON_EFFECT_NIGHT_VISION,
        Hunger = FALCON_EFFECT_HUNGER,
        Weakness = FALCON_EFFECT_WEAKNESS,
        Poison = FALCON_EFFECT_POISON,
        Wither = FALCON_EFFECT_WITHER,
        HealthBoost = FALCON_EFFECT_HEALTH_BOOST,
        Absorption = FALCON_EFFECT_ABSORPTION,
        Saturation = FALCON_EFFECT_SATURATION,
        Levitation = FALCON_EFFECT_LEVITATION,
        FatalPoison = FALCON_EFFECT_FATAL_POISON,
        ConduitPower = FALCON_EFFECT_CONDUIT_POWER,
        SlowFalling = FALCON_EFFECT_SLOW_FALLING,
        BadOmen = FALCON_EFFECT_BAD_OMEN,
        VillageHero = FALCON_EFFECT_VILLAGE_HERO,
        Darkness = FALCON_EFFECT_DARKNESS,
        Weaving = FALCON_EFFECT_WEAVING,
        RaidOmen = FALCON_EFFECT_RAID_OMEN,
        Freezing = FALCON_EFFECT_FREEZING
    };

    struct Effect {
        static constexpr int32_t INFINITE = FALCON_EFFECT_INFINITE;

        EffectType type = EffectType::Speed;
        /** Ticks left, INFINITE for an endless effect. */
        int32_t duration = 0;
        int32_t amplifier = 0;
        bool showParticles = true;
        bool ambient = false;

        static Effect from(const FalconEffect &value) {
            return Effect{static_cast<EffectType>(value.type), value.duration, value.amplifier,
                          value.showParticles != 0, value.ambient != 0};
        }
    };

    struct Attribute {
        std::string name;
        float value = 0.0f;
        float minimum = 0.0f;
        float maximum = 0.0f;
        float defaultValue = 0.0f;
    };
}

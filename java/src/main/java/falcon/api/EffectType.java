package falcon.api;

import falcon.api.internal.FalconAbi;

public enum EffectType {
    SPEED(FalconAbi.EFFECT_SPEED),
    SLOWNESS(FalconAbi.EFFECT_SLOWNESS),
    HASTE(FalconAbi.EFFECT_HASTE),
    MINING_FATIGUE(FalconAbi.EFFECT_MINING_FATIGUE),
    STRENGTH(FalconAbi.EFFECT_STRENGTH),
    INSTANT_HEALTH(FalconAbi.EFFECT_INSTANT_HEALTH),
    INSTANT_DAMAGE(FalconAbi.EFFECT_INSTANT_DAMAGE),
    JUMP_BOOST(FalconAbi.EFFECT_JUMP_BOOST),
    NAUSEA(FalconAbi.EFFECT_NAUSEA),
    REGENERATION(FalconAbi.EFFECT_REGENERATION),
    RESISTANCE(FalconAbi.EFFECT_RESISTANCE),
    FIRE_RESISTANCE(FalconAbi.EFFECT_FIRE_RESISTANCE),
    WATER_BREATHING(FalconAbi.EFFECT_WATER_BREATHING),
    INVISIBILITY(FalconAbi.EFFECT_INVISIBILITY),
    BLINDNESS(FalconAbi.EFFECT_BLINDNESS),
    NIGHT_VISION(FalconAbi.EFFECT_NIGHT_VISION),
    HUNGER(FalconAbi.EFFECT_HUNGER),
    WEAKNESS(FalconAbi.EFFECT_WEAKNESS),
    POISON(FalconAbi.EFFECT_POISON),
    WITHER(FalconAbi.EFFECT_WITHER),
    HEALTH_BOOST(FalconAbi.EFFECT_HEALTH_BOOST),
    ABSORPTION(FalconAbi.EFFECT_ABSORPTION),
    SATURATION(FalconAbi.EFFECT_SATURATION),
    LEVITATION(FalconAbi.EFFECT_LEVITATION),
    FATAL_POISON(FalconAbi.EFFECT_FATAL_POISON),
    CONDUIT_POWER(FalconAbi.EFFECT_CONDUIT_POWER),
    SLOW_FALLING(FalconAbi.EFFECT_SLOW_FALLING),
    BAD_OMEN(FalconAbi.EFFECT_BAD_OMEN),
    VILLAGE_HERO(FalconAbi.EFFECT_VILLAGE_HERO),
    DARKNESS(FalconAbi.EFFECT_DARKNESS),
    WEAVING(FalconAbi.EFFECT_WEAVING),
    RAID_OMEN(FalconAbi.EFFECT_RAID_OMEN),
    FREEZING(FalconAbi.EFFECT_FREEZING);

    private final int mValue;

    EffectType(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static EffectType of(int value) {
        for (EffectType type : values()) {
            if (type.mValue == value) {
                return type;
            }
        }
        throw new IllegalArgumentException("Unknown effect " + value);
    }
}

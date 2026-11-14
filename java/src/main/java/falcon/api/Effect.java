package falcon.api;

import falcon.api.internal.FalconAbi;

import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

/**
 * A potion effect on an entity. The duration is in ticks, {@link #INFINITE} for an endless effect.
 */
public record Effect(EffectType type, int duration, int amplifier, boolean showParticles, boolean ambient) {
    public static final int INFINITE = FalconAbi.EFFECT_INFINITE;

    static Effect read(MemorySegment segment) {
        return new Effect(EffectType.of(segment.get(ValueLayout.JAVA_INT, FalconAbi.Effect.TYPE)),
                segment.get(ValueLayout.JAVA_INT, FalconAbi.Effect.DURATION),
                segment.get(ValueLayout.JAVA_INT, FalconAbi.Effect.AMPLIFIER),
                segment.get(ValueLayout.JAVA_INT, FalconAbi.Effect.SHOW_PARTICLES) != 0,
                segment.get(ValueLayout.JAVA_INT, FalconAbi.Effect.AMBIENT) != 0);
    }
}

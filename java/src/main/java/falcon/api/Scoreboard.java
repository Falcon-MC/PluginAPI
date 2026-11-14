package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.OptionalInt;

/**
 * The server's scoreboard, shared with commands and scripts and saved with the default world. Objectives are dummy
 * objectives. A participant is an entity, a player through its entity, or a fake player named by text.
 */
public final class Scoreboard {
    private final PluginContext mContext;

    Scoreboard(PluginContext context) {
        mContext = context;
    }

    /** Returns false when the name is taken. */
    public boolean addObjective(String name, String displayName) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardAddObjective(Interop.text(arena, name), Interop.text(arena, displayName)) != 0;
        }
    }

    public boolean removeObjective(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardRemoveObjective(Interop.text(arena, name)) != 0;
        }
    }

    public List<String> objectives() {
        int count = api().scoreboardObjectiveCount();
        List<String> names = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().scoreboardObjectiveName(index)));
        }
        return names;
    }

    public Optional<String> displayName(String objective) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment name = api().scoreboardObjectiveDisplayName(Interop.text(arena, objective));
            return Interop.isNull(name) ? Optional.empty() : Optional.of(Interop.string(name));
        }
    }

    public OptionalInt score(String objective, Entity entity) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment value = arena.allocate(ValueLayout.JAVA_INT);
            if (api().scoreboardEntityScore(Interop.text(arena, objective), entity.handle(), value) == 0) {
                return OptionalInt.empty();
            }
            return OptionalInt.of(value.get(ValueLayout.JAVA_INT, 0));
        }
    }

    public boolean setScore(String objective, Entity entity, int value) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardSetEntityScore(Interop.text(arena, objective), entity.handle(), value) != 0;
        }
    }

    public boolean resetScore(String objective, Entity entity) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardResetEntityScore(Interop.text(arena, objective), entity.handle()) != 0;
        }
    }

    public OptionalInt score(String objective, String fakePlayer) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment value = arena.allocate(ValueLayout.JAVA_INT);
            if (api().scoreboardFakePlayerScore(Interop.text(arena, objective), Interop.text(arena, fakePlayer),
                    value) == 0) {
                return OptionalInt.empty();
            }
            return OptionalInt.of(value.get(ValueLayout.JAVA_INT, 0));
        }
    }

    public boolean setScore(String objective, String fakePlayer, int value) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardSetFakePlayerScore(Interop.text(arena, objective), Interop.text(arena, fakePlayer),
                    value) != 0;
        }
    }

    public boolean resetScore(String objective, String fakePlayer) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardResetFakePlayerScore(Interop.text(arena, objective),
                    Interop.text(arena, fakePlayer)) != 0;
        }
    }

    /** Every score of an objective, in the order the participants joined it. */
    public List<Score> scores(String objective) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment name = Interop.text(arena, objective);
            MemorySegment value = arena.allocate(ValueLayout.JAVA_INT);
            int count = api().scoreboardScoreCount(name);
            List<Score> result = new ArrayList<>(Math.max(count, 0));
            for (int index = 0; index < count; index++) {
                MemorySegment participant = api().scoreboardScoreAt(name, index, value);
                if (Interop.isNull(participant)) {
                    continue;
                }
                MemorySegment holder = api().scoreboardScoreEntity(name, index);
                Optional<Entity> entity = Interop.isNull(holder) ? Optional.empty()
                                                                 : Optional.of(new Entity(mContext, holder));
                result.add(new Score(Interop.string(participant), value.get(ValueLayout.JAVA_INT, 0), entity));
            }
            return result;
        }
    }

    public boolean setDisplay(DisplaySlot slot, String objective) {
        return setDisplay(slot, objective, false);
    }

    /** Shows an objective in a slot, highest score first unless ascending. */
    public boolean setDisplay(DisplaySlot slot, String objective, boolean ascending) {
        try (Arena arena = Arena.ofConfined()) {
            return api().scoreboardSetDisplay(slot.value(), Interop.text(arena, objective), ascending ? 1 : 0) != 0;
        }
    }

    public void clearDisplay(DisplaySlot slot) {
        api().scoreboardSetDisplay(slot.value(), MemorySegment.NULL, 0);
    }

    public Optional<String> displayed(DisplaySlot slot) {
        MemorySegment name = api().scoreboardDisplayObjective(slot.value());
        return Interop.isNull(name) ? Optional.empty() : Optional.of(Interop.string(name));
    }

    private static NativeApi api() {
        return Interop.api();
    }
}

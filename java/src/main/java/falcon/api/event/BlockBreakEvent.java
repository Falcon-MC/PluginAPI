package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Player;
import falcon.api.Item;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.List;

public final class BlockBreakEvent extends Event {
    BlockBreakEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public String blockName() {
        return eventBlockName();
    }

    /** The stacks are borrowed until the event ends or the drops are replaced. */
    public List<Item> drops() {
        return eventDrops();
    }

    public void setDrops(List<Item> drops) {
        eventSetDrops(drops);
    }

    public int experience() {
        return api().eventExperience(mHandle);
    }

    public void setExperience(int experience) {
        api().eventSetExperience(mHandle, experience);
    }
}

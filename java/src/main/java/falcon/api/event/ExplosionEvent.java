package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Entity;
import falcon.api.Level;
import falcon.api.Vec3;
import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public final class ExplosionEvent extends Event {
    ExplosionEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Optional<Entity> source() {
        return optionalEntity(api().eventEntity(mHandle));
    }

    public Level level() {
        return eventLevel();
    }

    public Vec3 position() {
        return eventPosition();
    }

    public double power() {
        return api().eventAmount(mHandle);
    }

    public void setPower(double power) {
        api().eventSetAmount(mHandle, power);
    }

    public List<BlockPos> blocks() {
        int count = api().eventBlockCount(mHandle);
        List<BlockPos> result = new ArrayList<>(Math.max(count, 0));
        try (Arena arena = Arena.ofConfined()) {
            for (int index = 0; index < count; index++) {
                result.add(Interop.blockPos(api().eventBlockAt(arena, mHandle, index)));
            }
        }
        return result;
    }

    public void setBlocks(List<BlockPos> blocks) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment positions = MemorySegment.NULL;
            if (!blocks.isEmpty()) {
                long stride = FalconAbi.BlockPos.LAYOUT.byteSize();
                positions = arena.allocate(FalconAbi.BlockPos.LAYOUT, blocks.size());
                for (int index = 0; index < blocks.size(); index++) {
                    Interop.writeBlockPos(positions.asSlice(stride * index, stride), blocks.get(index));
                }
            }
            api().eventSetBlocks(mHandle, positions, blocks.size());
        }
    }
}

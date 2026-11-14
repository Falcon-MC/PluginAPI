package falcon.api;

import falcon.api.internal.FalconAbi;

public enum BlockFace {
    DOWN(FalconAbi.FACE_DOWN),
    UP(FalconAbi.FACE_UP),
    NORTH(FalconAbi.FACE_NORTH),
    SOUTH(FalconAbi.FACE_SOUTH),
    WEST(FalconAbi.FACE_WEST),
    EAST(FalconAbi.FACE_EAST);

    private final int mValue;

    BlockFace(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static BlockFace of(int value) {
        for (BlockFace entry : values()) {
            if (entry.mValue == value) {
                return entry;
            }
        }
        throw new IllegalArgumentException("Unknown value " + value);
    }
}

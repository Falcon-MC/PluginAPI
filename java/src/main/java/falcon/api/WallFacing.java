package falcon.api;

/** The way the front of the frames points, the facing_direction of an item frame. */
public enum WallFacing {
    NORTH(2),
    SOUTH(3),
    WEST(4),
    EAST(5);

    private final int mValue;

    WallFacing(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}

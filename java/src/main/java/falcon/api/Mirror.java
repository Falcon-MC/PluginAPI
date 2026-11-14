package falcon.api;

import falcon.api.internal.FalconAbi;

public enum Mirror {
    NONE(FalconAbi.MIRROR_NONE),
    X(FalconAbi.MIRROR_X),
    Z(FalconAbi.MIRROR_Z),
    XZ(FalconAbi.MIRROR_XZ);

    private final int mValue;

    Mirror(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static Mirror of(int value) {
        for (Mirror entry : values()) {
            if (entry.mValue == value) {
                return entry;
            }
        }
        throw new IllegalArgumentException("Unknown value " + value);
    }
}

package falcon.api;

import falcon.api.internal.FalconAbi;

public enum Dimension {
    OVERWORLD(FalconAbi.DIMENSION_OVERWORLD),
    NETHER(FalconAbi.DIMENSION_NETHER),
    THE_END(FalconAbi.DIMENSION_THE_END);

    private final int mValue;

    Dimension(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static Dimension of(int value) {
        for (Dimension dimension : values()) {
            if (dimension.mValue == value) {
                return dimension;
            }
        }
        throw new IllegalArgumentException("Unknown dimension " + value);
    }
}
